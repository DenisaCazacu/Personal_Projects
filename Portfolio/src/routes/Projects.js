import React from 'react'
import Footer from "./../Components/Footer.js"
import Navbar from "./../Components/Navbar.js"
import ProjectsCarousel from "../Components/ProjectsCarousel.js"
import "./ProjectsStyle.css"

export const Projects = () => {
  return (
    <div className='projects-section'>
      <Navbar/>
      <ProjectsCarousel />
      <Footer/>
    </div>
  )
}

export default Projects;