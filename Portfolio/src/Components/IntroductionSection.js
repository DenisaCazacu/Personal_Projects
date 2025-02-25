import React from "react";
import "./IntroductionSectionStyle.css";
import "./ButtonsStyle.css";
import { Link } from "react-router-dom";
import { FaFolderOpen } from "react-icons/fa";
import { FaRegUser } from "react-icons/fa";

export const IntroductionSection = () => {
  return (
    <div className="introduction-section">
        <div className="introduction-container">
          <p className = "introduction-text"> Welcome to my portfolio! I'm a computer science student at Facultatea de Informatica Iasi,
            with a passion for leveraging technology to solve real-world problems. 
            Skilled in coding, algorithms, and software development. Seeking internship 
            opportunities to apply knowledge and contribute to the tech industry's innovation.
          </p>
          <div className="intro-buttons">
            <Link to="/about">
              <div className="button">
                <FaRegUser className="icon"/>
                Find more about me</div>
            </Link>
            <Link to="/projects">
              <div className="button">
              <FaFolderOpen className="icon"/>
              See my projects</div>
            </Link>
          </div>
        </div>
    </div>
  )
}

export default IntroductionSection;