use colored::*;
use rustls::pki_types::pem::PemObject;
use rustls::pki_types::{CertificateDer, PrivateKeyDer};
use rustls::ServerConfig;
use serde::{Deserialize, Serialize};
use sqlx::sqlite::SqlitePool;
use sqlx::FromRow;
use std::collections::HashMap;
use std::sync::Arc; //for handling shared references between clients
use tokio::io::{AsyncReadExt, AsyncWriteExt, WriteHalf};
use tokio::net::{TcpListener, TcpStream};
use tokio::sync::Mutex;
use tokio_rustls::server::TlsStream;
use tokio_rustls::TlsAcceptor;

type UserMap = Arc<Mutex<HashMap<String, Arc<Mutex<WriteHalf<TlsStream<TcpStream>>>>>>>;

#[derive(Debug, Serialize, Deserialize, FromRow)]
struct Message {
    sender: String,
    receiver: String,
    content: String,
}

#[tokio::main]
async fn main() -> tokio::io::Result<()> {
    // TLS configuration
    let server_config = create_server_config();
    let acceptor = TlsAcceptor::from(Arc::new(server_config));

    // TCP server listner
    let listener = TcpListener::bind("127.0.0.1:8090").await?;
    // Database connection
    let pool = Arc::new(
        SqlitePool::connect("sqlite://./users.sqlite")
            .await
            .unwrap(),
    );
    // Struct to store the connected users
    let connected_users: UserMap = Arc::new(Mutex::new(HashMap::new()));

    println!("Server running on 127.0.0.1:8090");

    loop {
        let (socket, addr) = listener.accept().await?;
        println!("{}{:?}", "Client connected at the address: ".yellow(), addr);
        let acceptor = acceptor.clone();
        let users_clone = Arc::clone(&connected_users);
        let pool_clone = Arc::clone(&pool);

        tokio::spawn(async move {
            let stream = acceptor.accept(socket).await;
            match stream {
                Ok(tls_stream) => {
                    if let Err(e) = handle_tls_client(tls_stream, users_clone, pool_clone).await {
                        eprintln!("Error handling client: {:?}", e);
                    }
                }
                Err(e) => eprintln!("TLS error: {:?}", e),
            }
        });
    }
}

async fn handle_tls_client(
    tls_stream: TlsStream<TcpStream>,
    users: UserMap,
    pool: Arc<SqlitePool>,
) -> tokio::io::Result<()> {
    let (mut reader, mut writer) = tokio::io::split(tls_stream);
    let mut current_user: String = String::new();
    match authenticate_client(&mut reader, &mut writer, Arc::clone(&pool)).await {
        Ok(Some(username)) => {
            current_user = username.clone();
            println! {"{} {:?} {}", "User".yellow(), username.clone(),"authenticated".yellow()};

            let writer_arc = Arc::new(Mutex::new(writer));
            {
                let mut users_lock = users.lock().await;
                users_lock.insert(username.clone(), writer_arc.clone());
            }
            match handle_client(
                &mut reader,
                writer_arc.clone(),
                users.clone(),
                pool,
                username.clone(),
            )
            .await
            {
                Ok(_) => {
                    println!("Client disconnected");
                }
                Err(e)
                    if e.kind() == std::io::ErrorKind::ConnectionReset
                        || e.kind() == std::io::ErrorKind::ConnectionAborted =>
                {
                    println!("Client {} disconnected.", current_user);
                }
                Err(e) => {
                    eprintln!("Error during client handling for {}: {e}", current_user);
                }
            }

            disconnect_user(&users, &username).await;
        }
        Ok(None) => {
            println!("Client disconnected during authentication.");
            return Ok(());
        }
        Err(e)
            if e.kind() == std::io::ErrorKind::ConnectionReset
                || e.kind() == std::io::ErrorKind::ConnectionAborted =>
        {
            println!("Client {} disconnected.", current_user.clone());
        }
        Err(e) => {
            eprintln!(
                "Error during client handling for {}: {e}",
                current_user.clone()
            );
        }
    }

    Ok(())
}

async fn authenticate_client(
    reader: &mut (impl AsyncReadExt + Unpin),
    writer: &mut (impl AsyncWriteExt + Unpin),
    pool: Arc<SqlitePool>,
) -> tokio::io::Result<Option<String>> {
    let mut buffer = vec![0u8; 1024];

    loop {
        writer
            .write_all(b"Please type 'login' or 'signin' for authentication.\n") // b = byte string literal (slice)
            .await?;
        let n = reader.read(&mut buffer).await?;
        if n == 0 {
            return Ok(None); // client disconnected
        }
        let choice = String::from_utf8_lossy(&buffer[..n]).trim().to_string(); // lossy for invalid utf-8

        match choice.as_str() {
            "login" => {
                writer.write_all(b"Username: \n").await?;
                let n = reader.read(&mut buffer).await?;
                if n == 0 {
                    return Ok(None);
                }
                let username = String::from_utf8_lossy(&buffer[..n]).trim().to_string();

                writer.write_all(b"Password: \n").await?;
                let n = reader.read(&mut buffer).await?;
                if n == 0 {
                    return Ok(None);
                }
                let password = String::from_utf8_lossy(&buffer[..n]).trim().to_string();

                match sqlx::query_scalar::<_, String>(
                    "SELECT username FROM users WHERE username = ? AND password = ?",
                )
                .bind(&username)
                .bind(&password)
                .fetch_optional(pool.as_ref()) // returns an Option<User> if found and None if not found
                .await
                {
                    Ok(Some(db_username)) => {
                        writer.write_all(b"Login successful\n").await?;
                        return Ok(Some(db_username));
                    }
                    Ok(None) => {
                        writer
                            .write_all(b"Invalid username or password. Try again.\n")
                            .await?;
                    }
                    Err(e) => {
                        eprintln!("Database users error: {e}");
                        writer.write_all(b"Database error, please retry.\n").await?;
                    }
                }
            }
            "signin" => {
                // Ask for username
                writer.write_all(b"Username: \n").await?;
                let n = reader.read(&mut buffer).await?;
                if n == 0 {
                    return Ok(None);
                }
                let username = String::from_utf8_lossy(&buffer[..n]).trim().to_string();

                // Ask for password
                writer.write_all(b"Password: \n").await?;
                let n = reader.read(&mut buffer).await?;
                if n == 0 {
                    return Ok(None);
                }
                let password = String::from_utf8_lossy(&buffer[..n]).trim().to_string();

                // Adding the user to the database ( username is unique, so no need for checking )
                match sqlx::query("INSERT INTO users (username, password) VALUES (?, ?)")
                    .bind(&username)
                    .bind(&password)
                    .execute(pool.as_ref())
                    .await
                {
                    Ok(_) => {
                        writer.write_all(b"Signin successful\n").await?;
                        return Ok(Some(username));
                    }
                    Err(e) => {
                        eprintln!("Database users error: {e}");
                        writer
                            .write_all(b"Error during signup, username might be taken.\n")
                            .await?;
                    }
                }
            }
            _ => {
                writer
                    .write_all(b"Invalid choice. Please choose 'login' or 'signin'.\n")
                    .await?;
            }
        }
    }
}

async fn handle_client(
    reader: &mut (impl AsyncReadExt + Unpin),
    writer: Arc<Mutex<WriteHalf<TlsStream<TcpStream>>>>,
    users: UserMap,
    pool: Arc<SqlitePool>,
    current_user: String,
) -> tokio::io::Result<()> {
    let mut buffer = vec![0u8; 1024];
    //sends the notifications it received while it was offline
    let unread_messages = get_unread_messages(Arc::clone(&pool), current_user.clone()).await?;
    for message in unread_messages {
        let serialized_message = serde_json::to_string(&message).unwrap();

        {
            let mut writer_guard = writer.lock().await;
            writer_guard
                .write_all(format!("{}\n", serialized_message).as_bytes())
                .await?;
        }
    }
    mark_messages_as_read(Arc::clone(&pool), current_user.clone()).await?;

    loop {
        let n = reader.read(&mut buffer).await?;

        if n == 0 {
            return Ok(()); // client disconnected
        }
        let message_str = String::from_utf8_lossy(&buffer[..n]).to_string();
        println!("Received message: {}", message_str.purple());

        //verify if it is a command: history, reply or help
        if message_str.trim().starts_with("history") {
            //parse the message to get the user
            if let Some((_, username_history)) = parse_message(&message_str) {
                let history = get_history(
                    Arc::clone(&pool),
                    current_user.clone(),
                    username_history.clone(),
                )
                .await?;
                for message in history {
                    let serialized_message = serde_json::to_string(&message).unwrap();

                    {
                        let mut writer_guard = writer.lock().await;
                        writer_guard
                            .write_all(format!("{}\n", serialized_message).as_bytes())
                            .await?;
                    }
                }
            } else {
                {
                    let mut writer_guard = writer.lock().await;
                    writer_guard
                        .write_all(
                            b"Invalid message format. Please use the format: 'history: username'\n",
                        )
                        .await?;
                }
            }
            continue;
        } else if message_str.trim().starts_with("help") {
            {
                let mut writer_guard = writer.lock().await;
                writer_guard.write_all(b"Commands: \n").await?;
                writer_guard
                    .write_all(b"- <username>:<message> -> sends a message to user\n")
                    .await?;
                writer_guard
                    .write_all(b"- history: <username> -> to see all the messages\n")
                    .await?;
                writer_guard
                .write_all(
                    b"- reply: '<username>:<message>': <your_message> -> to reply to a message\n",
                )
                .await?;
                writer_guard
                    .write_all(b"- help -> to see the commands\n")
                    .await?;
                writer_guard
                    .write_all(b"- quit -> to exit the chat\n")
                    .await?;
            }

            continue;
        } else if message_str.trim().starts_with("quit") {
            return Ok(());
        } else if message_str.trim().starts_with("reply") {
            if let Some((user_to_reply, message_to_reply, your_message)) =
                parse_reply_message(&message_str)
            {
                let formatted_content =
                    format!("Replied to '{}': {}", message_to_reply, your_message);
                let message = Message {
                    sender: current_user.clone(),
                    receiver: user_to_reply.clone(),
                    content: formatted_content.clone(),
                };
                if message_exists(
                    Arc::clone(&pool),
                    user_to_reply.clone(),
                    current_user.clone(),
                    message_to_reply.clone(),
                )
                .await?
                {
                    match send_message(
                        Arc::clone(&users),
                        message,
                        Arc::clone(&pool),
                        current_user.clone(),
                    )
                    .await
                    {
                        Ok(_) => {}
                        Err(e) => match e.kind() {
                            std::io::ErrorKind::Other if e.to_string() == "NotFound" => {
                                println!("User does not exist.");

                                {
                                    let mut writer_guard = writer.lock().await;
                                    writer_guard.write_all(b"User does not exist\n").await?;
                                }
                            }
                            _ => {
                                println!("Failed to send message due to an unexpected error: {e}");
                            }
                        },
                    }
                } else {
                    println!("This message does not exist.");

                    {
                        let mut writer_guard = writer.lock().await;
                        writer_guard
                            .write_all(b"This message does not exist\n")
                            .await?;
                    }
                }
            } else {
                {
                    let mut writer_guard = writer.lock().await;
                    writer_guard
                        .write_all(
                            b"Invalid reply format. Use: reply 'user: message' : your_message\n",
                        )
                        .await?;
                }
            }
        } else {
            //if it is not a command, it should be a message
            //parse the message
            if let Some((user, content)) = parse_message(&message_str) {
                let message = Message {
                    sender: current_user.clone(),
                    receiver: user.clone(),
                    content: content.clone(),
                };

                match send_message(
                    Arc::clone(&users),
                    message,
                    Arc::clone(&pool),
                    current_user.clone(),
                )
                .await
                {
                    Ok(_) => {}
                    Err(e) => match e.kind() {
                        std::io::ErrorKind::Other if e.to_string() == "NotFound" => {
                            println!("User does not exist.");

                            {
                                let mut writer_guard = writer.lock().await;
                                writer_guard.write_all(b"User does not exist\n").await?;
                            }
                        }
                        _ => {
                            println!("Failed to send message due to an unexpected error: {e}");
                        }
                    },
                }
            } else {
                //if it is not a valid message, send an error message

                {
                    let mut writer_guard = writer.lock().await;
                    writer_guard
                        .write_all(
                            b"Invalid message format. Please use the format: 'username: message'\n",
                        )
                        .await?;
                }
            }
        }
    }
}

fn parse_message(message: &str) -> Option<(String, String)> {
    let parts: Vec<&str> = message.splitn(2, ':').collect();
    if parts.len() == 2 {
        let part1 = parts[0].trim().to_string();
        let part2 = parts[1].trim().to_string();
        Some((part1, part2))
    } else {
        None
    }
}

async fn save_message_in_database(
    pool: Arc<SqlitePool>,
    sender_username: String,
    receiver_username: String,
    content: &str,
    is_read: bool,
) -> tokio::io::Result<()> {
    match sqlx::query(
        "INSERT INTO messages (sender_username, receiver_username, content, is_read) VALUES (?, ?, ?, ?)",
    )
    .bind(sender_username)
    .bind(receiver_username)
    .bind(content)
    .bind(is_read)
    .execute(pool.as_ref())
    .await
    {
        Ok(_) => {
            return Ok(());
        }
        Err(e) => {
            eprintln!("Database users error: {e}");
        }
    }
    Ok(())
}

async fn get_unread_messages(
    pool: Arc<SqlitePool>,
    username: String,
) -> tokio::io::Result<Vec<Message>> {
    let messages = sqlx::query_as::<_, Message>(
        "SELECT sender_username AS sender, receiver_username AS receiver, content 
         FROM messages WHERE receiver_username = ? AND is_read = 0",
    )
    .bind(&username)
    .fetch_all(pool.as_ref())
    .await
    .map_err(|e| {
        eprintln!("Database error while fetching unread messages: {e}");
        std::io::Error::new(std::io::ErrorKind::Other, e)
    })?;
    Ok(messages)
}

async fn mark_messages_as_read(pool: Arc<SqlitePool>, username: String) -> tokio::io::Result<()> {
    sqlx::query("UPDATE messages SET is_read = 1 WHERE receiver_username = ? AND is_read = 0")
        .bind(&username)
        .execute(pool.as_ref())
        .await
        .map_err(|e| {
            eprintln!("Database error while marking messages as read: {e}");
            std::io::Error::new(std::io::ErrorKind::Other, e)
        })?;
    Ok(())
}

async fn disconnect_user(users: &UserMap, username: &str) {
    let mut users_lock = users.lock().await;
    if users_lock.remove(username).is_some() {
        println!("Removed user {} from connected users.", username);
    } else {
        println!("User {} was not found in connected users.", username);
    }
}

async fn get_history(
    pool: Arc<SqlitePool>,
    username1: String,
    username2: String,
) -> tokio::io::Result<Vec<Message>> {
    let messages = sqlx::query_as::<_, Message>(
        "SELECT sender_username AS sender, receiver_username AS receiver, content 
         FROM messages WHERE (sender_username = ? AND receiver_username = ?)
            OR (sender_username = ? AND receiver_username = ?) 
         ORDER BY id",
    )
    .bind(&username1)
    .bind(&username2)
    .bind(&username2)
    .bind(&username1)
    .fetch_all(pool.as_ref())
    .await
    .map_err(|e| {
        eprintln!("Database error while fetching messages: {e}");
        std::io::Error::new(std::io::ErrorKind::Other, e)
    })?;
    Ok(messages)
}

fn parse_reply_message(message: &str) -> Option<(String, String, String)> {
    // messages like "reply: 'user: message': your_message"
    let parts: Vec<&str> = message.splitn(3, '\'').collect();
    if parts.len() == 3 {
        // first part: "reply: " is not needed
        let user_and_message = parts[1].trim(); // second part: "user: message"
        let your_message = parts[2].trim().trim_start_matches(':').trim(); // third part: "your_message"
        let user_and_message_parts: Vec<&str> = user_and_message.splitn(2, ':').collect();
        if user_and_message_parts.len() == 2 {
            let user_to_reply = user_and_message_parts[0].trim().to_string();
            let message_to_reply = user_and_message_parts[1].trim().to_string();
            return Some((user_to_reply, message_to_reply, your_message.to_string()));
        }
    }

    None
}

async fn send_message(
    users: UserMap,
    message: Message,
    pool: Arc<SqlitePool>,
    current_user: String,
) -> tokio::io::Result<()> {
    //search for the user in the hashmap
    let users = users.lock().await;
    if let Some(receiver) = users.get(&message.receiver) {
        // if the user is connected, send the message
        {
            let mut writer = receiver.lock().await;
            let serialized_message = serde_json::to_string(&message).unwrap();
            writer
                .write_all(format!("{}\n", serialized_message).as_bytes())
                .await?;
            writer.flush().await?;
            println! {"Message sent to {}", message.receiver};
        }

        // save the message in the database
        save_message_in_database(
            Arc::clone(&pool),
            current_user.clone(),
            message.receiver.clone(),
            &message.content,
            true,
        )
        .await?;
    } else if user_exists(Arc::clone(&pool), message.receiver.clone()).await? {
        // if the user is not found, save the message in the database
        save_message_in_database(
            Arc::clone(&pool),
            current_user.clone(),
            message.receiver.clone(),
            &message.content,
            false,
        )
        .await?;
        println!(
            "User {} is not online, saving the message in the database",
            message.receiver
        );
    } else {
        return Err(std::io::Error::new(std::io::ErrorKind::Other, "NotFound"));
    }
    Ok(())
}

async fn user_exists(pool: Arc<SqlitePool>, username: String) -> tokio::io::Result<bool> {
    let result = sqlx::query_scalar::<_, i64>("SELECT COUNT(*) FROM users WHERE username = ?")
        .bind(username)
        .fetch_one(pool.as_ref())
        .await;

    match result {
        Ok(count) => Ok(count > 0),
        Err(e) => {
            eprintln!("Database error while checking user existence: {e}");
            Err(std::io::Error::new(std::io::ErrorKind::Other, e))
        }
    }
}

async fn message_exists(
    pool: Arc<SqlitePool>,
    username_sender: String,
    username_receiver: String,
    content: String,
) -> tokio::io::Result<bool> {
    let result = sqlx::query_scalar::<_, i64>("SELECT COUNT(*) FROM messages WHERE sender_username = ? AND receiver_username = ? AND content = ?")
        .bind(username_sender)
        .bind(username_receiver)
        .bind(content)
        .fetch_one(pool.as_ref())
        .await;

    match result {
        Ok(count) => {
            println!("result: {}", count);
            Ok(count > 0)
        }
        Err(e) => {
            eprintln!("Database error while checking user existence: {e}");
            Err(std::io::Error::new(std::io::ErrorKind::Other, e))
        }
    }
}

fn create_server_config() -> ServerConfig {
    let certs = CertificateDer::pem_file_iter("server.crt")
        .expect("cannot open certificate file")
        .map(|cert| cert.unwrap())
        .collect::<Vec<_>>();
    let private_key =
        PrivateKeyDer::from_pem_file("server.key").expect("cannot open private key file");

    ServerConfig::builder()
        .with_no_client_auth()
        .with_single_cert(certs, private_key)
        .unwrap()
}
