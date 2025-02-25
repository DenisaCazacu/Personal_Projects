import React from 'react'
import Footer from "./../Components/Footer.js"
import Navbar from "./../Components/Navbar.js"
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faBuildingColumns } from "@fortawesome/free-solid-svg-icons";
import illustration1 from "../Images/ilustration1.svg";
import illustration2 from "../Images/ilustration2.svg";
import illustration3 from "../Images/ilustration3.svg";
import illustration4 from "../Images/ilustration4.svg";
import illustration5 from "../Images/illustration-books.svg";
import illustration6 from "../Images/illustration-travel.svg";
import illustration7 from "../Images/illustration-music.svg";
import "./AboutMeStyle.css"

export const AboutMe = () => {
  return (
    <div className='about-me-section'>
      <Navbar/>
      <div className="about-me-container">
        <div className='education box'>
          <h2 className="section-title">Education</h2>
          <p className="paragraph-text">
            <FontAwesomeIcon icon={faBuildingColumns} size={30} style={{ color: "white" }} />
            &nbsp; Faculty of Computer Science Iasi (Oct. 2023 – Present) <br/>
                                        ”Alexandru Ioan Cuza” University <br/>
                                        Courses:
                                        <ul>
                                          <li>Data Structures</li>
                                          <li>Algorithm Design</li>
                                          <li>Object-Oriented Programming</li>
                                          <li>Operating Systems </li>
                                          <li>Computer Networks</li>
                                          <li>Databases</li>
                                          <li>Rust Programming</li>
                                        </ul>
            <img className="illustration" src={illustration4} alt="illustration4" />
          </p>
        </div>
        <div className="hard-skills box">
          <h2 className="section-title">Technical Skills</h2>
          <img className="illustration" src={illustration1} alt="illustration1" />
          <p className="paragraph-text">Languages / Frameworks </p>
          <p className="paragraph-text-2"> C/C++, Rust, HTML, CSS, React, SQL</p>
          <img className="illustration" src={illustration2} alt="illustration2" />
          <p className="paragraph-text">Developer Tools</p>
          <p className="paragraph-text-2"> Git, VS Code, Visual Studio, Figma </p>
        </div>
        <div className="soft-skills box">
          <h2 className="section-title">Soft Skills</h2>
          <img className="illustration" src={illustration3} alt="illustration3" />
          <p className="paragraph-text">
            <ul>
              <li>Communication</li>
              <li>Teamwork</li>
              <li>Problem-solving</li>
              <li>Strategic thinking</li>
            </ul>
          </p>
        </div>
        <div className='interests box'>
          <h2 className="section-title">Interests</h2>
          <div className="interests-container">
            <div className="interest">
              <img className="illustration" src={illustration5} alt="illustration5" />
              <p>Reading</p>
            </div>
            <div className="interest">
              <img className="illustration" src={illustration6} alt="illustration6" />
              <p>Travel</p>
            </div>
            <div className="interest">
              <img className="illustration" src={illustration7} alt="illustration7" />
              <p>Music</p>
            </div>
          </div>
        </div>
      </div>
      <Footer />
    </div>
  )
}

export default AboutMe;