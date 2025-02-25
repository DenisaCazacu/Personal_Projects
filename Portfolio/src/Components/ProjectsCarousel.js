import React, { useState } from "react";
import { FaChevronLeft, FaChevronRight } from "react-icons/fa";
import "./ProjectsCarouselStyle.css";
import cppImage from "../Images/cpp.png";
import cImage from "../Images/c.png";
import rustImage from "../Images/rust.png";
import reactImage from "../Images/react.png";
import htmlcssImage from "../Images/html-css.png";
import QuizzGameDemo from "../Videos/QuizzGame.mp4";
import InterSchemDemo from "../Videos/InterSchem.mp4";
import OfflineMessengerDemo from "../Videos/OfflineMessenger.mp4";
import PortfolioDemo from "../Videos/Portfolio.mp4";
import CVWebsiteDemo from "../Videos/CVWebsite.mp4";
import { FaTimes } from "react-icons/fa";

const projects = [
  { id: 1, image: cImage, demo: QuizzGameDemo, title: "Quizz Game",
    description: "A server-client-based multiplayer application where players answer timed questions. Built using TCP, multithreading for handling multiple clients, SQLite for question storage, and Qt for a user-friendly interface." },
  { id: 2, image: cppImage, demo: InterSchemDemo, title: "InterSchem", 
    description: "It represents a graphical interpreter for logical algorithmic schemes. Implemented in C++, using basic elements of the graphical interface." },
  { id: 3, image: reactImage, demo: PortfolioDemo, title: "Portfolio Website", 
    description: "Dynamic and visually engaging platform designed to showcase projects, skills, and experiences in an interactive way. Built with React, it features a sleek and responsive design." },
  { id: 4, image: htmlcssImage, demo: CVWebsiteDemo, title: "CV Website", 
    description: "Basic site made in HTML and CSS, using Visual Studio Code." },
  { id: 5, image: rustImage, demo: OfflineMessengerDemo, title: "Offline Messenger", 
    description: "A client-server application built in Rust, using async operations, TLS for secure connections, and sqlx for storage. The server handles user authentication, message sending and receiving, and displays notifications upon reconnection." }
];

const ProjectsCarousel = () => {
  const [cards, setCards] = useState(projects);

  const shiftLeft = () => {
    setCards((prev) => [...prev.slice(1), prev[0]]);
  };

  const shiftRight = () => {
    setCards((prev) => [prev[prev.length - 1], ...prev.slice(0,prev.length-1)]);
  };

  const [selectedProject, setSelectedProject] = useState(null);
  const [modal, setModal] = useState(false);

  const toggleModal = (project = null) => {
    setSelectedProject(project);
    setModal(!modal);
  };

  if(modal) {
    document.body.classList.add('active-modal')
  } else {
    document.body.classList.remove('active-modal')
  }

  return (
    <div className="carousel-container">
      <div className="projects-button" onClick={shiftLeft}>
        <FaChevronLeft />
      </div>

      <div className="carousel">
        {cards.map((project, index) => (
          <div key={project.id} className={`card ${index === 2 ? "center" : ""}`}>
            <img src={project.image} alt={project.title} />
            <h2>{project.title}</h2>
            <p>{project.description}</p>
            <div onClick={() => toggleModal(project)} className="button">
              See more
            </div>
          </div>
        ))}
      </div>

      <div className="projects-button" onClick={shiftRight}>
        <FaChevronRight />
      </div>
      
      {modal && selectedProject && (
        <div className="modal">
          <div onClick={() => toggleModal(selectedProject)} className="overlay"></div>
          <div className="modal-content">
          <video controls autoplay loop muted>
            <source src={selectedProject.demo} type="video/mp4"/>
          </video>
            <FaTimes size={25} style={{ color: "black" }} className="close-modal" onClick={toggleModal} />
          </div>
        </div>
      )}
      
    </div>
  );
};

export default ProjectsCarousel;

