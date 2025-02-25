import "./NavbarStyles.css";
import React, { useState, useEffect } from "react";
import { Link, useLocation } from "react-router-dom";
import { FaBars, FaTimes } from "react-icons/fa";

export const Navbar = () => {
  const location = useLocation();
  const [clickHamburger, setClickHamburger] = useState(false);
  const [activeButton, setActiveButton] = useState("/"); 

  useEffect(() => {
    setActiveButton(location.pathname);
    setClickHamburger(false); 
  }, [location.pathname]);

  const handleClick = () => setClickHamburger(!clickHamburger);

  return (
    <div className="header">
        <div className="hamburger" onClick={handleClick}>
        {clickHamburger ? (
          <FaTimes size={20} style={{ color: "white" }} />
        ) : (
          <FaBars size={20} style={{ color: "white" }} />
        )}
      </div>
      <div className={clickHamburger ? "nav-menu active" : "nav-menu"}>
        <Link to="/">
          <button className={`nav-button ${activeButton === "/" ? "active" : ""}`}>
            Home
          </button>
        </Link>
        <Link to="/about">
          <button className={`nav-button ${activeButton === "/about" ? "active" : ""}`}>
            About Me
          </button>
        </Link>
        <Link to="/projects">
          <button className={`nav-button ${activeButton === "/projects" ? "active" : ""}`}>
            Projects
          </button>
        </Link>
        <Link to="/volunteering">
          <button className={`nav-button ${activeButton === "/volunteering" ? "active" : ""}`}>
            Volunteering
          </button>
        </Link>
        <Link to="/contact">
          <button className={`nav-button ${activeButton === "/contact" ? "active" : ""}`}>
            Contact
          </button>
        </Link>
      </div>
    </div>
  );
};

export default Navbar;
