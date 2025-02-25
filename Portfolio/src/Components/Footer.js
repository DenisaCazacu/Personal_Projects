import React from 'react'
import "./FooterStyle.css"
import "./ButtonsStyle.css"
import { FaHome } from "react-icons/fa"
import { FaMailBulk } from "react-icons/fa"
import { FaPhoneSquare  } from "react-icons/fa"
import { FaLinkedin } from "react-icons/fa";
import { FaGithub } from "react-icons/fa";
import { FaFacebook } from "react-icons/fa";

export const Footer = () => {
  return (
    <div className = "footer">
        <div className="footer-container">
        <div className="socials">
                <a href="https://www.linkedin.com/in/denisa-cazacu/" className="footer-button">
                    <FaLinkedin size={20} className='icon' />
                </a>
                <a href="https://github.com/DenisaCazacu" className="footer-button">
                    <FaGithub size={20} className='icon' />
                </a>
                <a href="https://www.facebook.com/profile.php?id=100009389260772" className="footer-button">
                    <FaFacebook size={20} className='icon' />
                </a>
            </div>
            <div className="info">
                <div className="info-item">
                    <FaHome size={25} style={{ color: "white" }} />
                    <p>Iasi, Romania</p>
                </div>
                <div className="info-item">
                    <FaMailBulk size={25} style={{ color: "white" }} />
                    <p> denisa.cazacu01@gmail.com</p>
                </div>
                <div className="info-item">
                    <FaPhoneSquare  size={25} style={{ color: "white" }} />
                    <p>0739912777</p>
                </div>
            </div>
        </div>
    </div>
  )
}

export default Footer;
