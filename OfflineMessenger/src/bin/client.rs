use colored::*;
use rustls::pki_types::pem::PemObject;
use rustls::pki_types::CertificateDer;
use rustls::pki_types::ServerName;
use rustls::{ClientConfig, RootCertStore};
use serde::{Deserialize, Serialize};
use std::io::ErrorKind;
use std::path::Path;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use tokio::io::{self, AsyncBufReadExt, AsyncReadExt, AsyncWriteExt, BufReader};
use tokio::io::{ReadHalf, WriteHalf};
use tokio::net::TcpStream;
use tokio_rustls::client::TlsStream;
use tokio_rustls::TlsConnector;

#[derive(Debug, Serialize, Deserialize)]
struct Message {
    sender: String,
    receiver: String,
    content: String,
}

#[tokio::main]
async fn main() -> tokio::io::Result<()> {
    let shutdown_flag = Arc::new(AtomicBool::new(false));

    //TLS config
    let cert_path = Path::new("server.crt");
    let root_cert_store = load_root_cert_store(cert_path);
    let config = ClientConfig::builder()
        .with_root_certificates(root_cert_store)
        .with_no_client_auth();

    let connector = TlsConnector::from(Arc::new(config));

    let stream = TcpStream::connect("127.0.0.1:8090").await?;

    let domain = ServerName::try_from("localhost").expect("Invalid domain");
    let mut tls_stream = connector
        .connect(domain, stream)
        .await
        .expect("Failed to establish TLS");

    authenticate_client(&mut tls_stream).await?;

    //let tls_stream = tls_stream.lock().await;
    let (reader, writer) = tokio::io::split(tls_stream);
    let shutdown_flag1 = Arc::clone(&shutdown_flag);
    tokio::spawn(async move {
        if let Err(e) = send_messages(writer, shutdown_flag1).await {
            eprintln!("Error in send_messages: {:?}", e);
        }
    });
    let shutdown_flag2 = Arc::clone(&shutdown_flag);
    tokio::spawn(async move {
        if let Err(e) = read_notifications(reader, shutdown_flag2).await {
            eprintln!("Error in read_notifications: {:?}", e);
        }
    });
    while !shutdown_flag.load(Ordering::SeqCst) {
        tokio::time::sleep(tokio::time::Duration::from_secs(1)).await;
    }
    Ok(())
}
async fn authenticate_client(stream: &mut TlsStream<TcpStream>) -> tokio::io::Result<()> {
    let mut buffer = [0; 1024];
    let mut input = String::new();

    loop {
        let n = stream.read(&mut buffer).await?;
        if n == 0 {
            break; // Connection closed
        }
        let received = String::from_utf8_lossy(&buffer[..n - 1]); //S -1 to remove the newline character
        println!("{}", received.green());

        if received.contains("Login successful") || received.contains("Signin successful") {
            break;
        }
        if received.contains("Username:")
            || received.contains("Password:")
            || received.contains("Please type 'login' or 'signin' for authentication.")
        {
            input.clear();
            std::io::stdin().read_line(&mut input)?;
            stream.write_all(input.trim().as_bytes()).await?
        }
    }

    Ok(())
}
async fn read_notifications(
    reader_tls: ReadHalf<TlsStream<TcpStream>>,
    shutdown_flag: Arc<AtomicBool>,
) -> tokio::io::Result<()> {
    let mut buffer = String::new();
    let mut reader = BufReader::new(reader_tls);
    loop {
        if shutdown_flag.load(Ordering::SeqCst) {
            break;
        }

        buffer.clear();

        match reader.read_line(&mut buffer).await {
            Ok(0) => {
                if shutdown_flag.load(Ordering::SeqCst) {
                    break;
                }
                return Err(std::io::Error::new(
                    std::io::ErrorKind::Other,
                    "Connection closed",
                ));
            }
            Ok(_) => {
                buffer = buffer.trim_end().to_string();

                match serde_json::from_str::<Message>(&buffer) {
                    Ok(message) => {
                        println!("{} : {}", message.sender.purple(), message.content.normal());
                    }
                    Err(_) => {
                        println!("{}: {}", "[Server]".red(), buffer.red());
                    }
                }
            }
            Err(e) => {
                if e.kind() == ErrorKind::UnexpectedEof {
                    println!("TLS connection closed.");
                    break;
                } else {
                    return Err(e);
                }
            }
        }
    }
    Ok(())
}
async fn send_messages(
    mut writer: WriteHalf<TlsStream<TcpStream>>,
    shutdown_flag: Arc<AtomicBool>,
) -> tokio::io::Result<()> {
    let stdin = io::stdin();
    let mut reader = BufReader::new(stdin);
    let mut input = String::new();

    loop {
        input.clear();
        reader.read_line(&mut input).await?;

        writer.write_all(input.trim().as_bytes()).await?;

        if input.trim().eq_ignore_ascii_case("quit") {
            println!("Exiting the chat...");
            shutdown_flag.store(true, Ordering::SeqCst);
            break;
        }
    }
    Ok(())
}

fn load_root_cert_store(path: &Path) -> RootCertStore {
    let mut root_cert_store = RootCertStore::empty();

    // Dacă avem un fișier specificat pentru certificatul rădăcină
    if path.exists() {
        root_cert_store.add_parsable_certificates(
            CertificateDer::pem_file_iter(path)
                .expect("Cannot open certificate file")
                .map(|result| result.expect("Failed to parse certificate")),
        );
    } else {
        println!("No root certificate found at {:?}", path);
    }

    root_cert_store
}
