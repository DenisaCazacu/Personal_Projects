import React from 'react'
import Footer from "./../Components/Footer.js"
import Navbar from "./../Components/Navbar.js"
import FiiCode from "../Images/FiiCode.png";
import BC from "../Images/BC.jpg";
import robotics from "../Images/robotics.png";
import FiiStudent from "../Images/FiiStudent.png";
import ASIILogo from "../Images/ASIILogo.png";
import mironobotLogo from "../Images/mironobotLogo.png";
import "./VolunteeringStyle.css"

export const Volunteering = () => {
  return (
    <div className='volunteering-section'>
      <Navbar/>
      <div className="volunteering-container">
        <div className='box'>
          <img className='logo' src= {ASIILogo} alt="Logo" />
          <p className="paragraph-text-2">Volunteer in the Internal Relations Department, developing my communication and
          organizational skills. I participated in the organization of several projects, such as:</p>
          <div className='cards'>
            <div class="polaroid polaroid1 orientation1">
              <img src= {FiiStudent} alt="FiiStudent" />
              <p class="caption">FiiStudent summer school</p>
            </div>
            <div class="polaroid polaroid1 orientation2">
              <img src= {BC} alt="BC" />
              <p class="caption">Charity Ball</p>
            </div>
            <div class="polaroid polaroid1 orientation1">
              <img src= {FiiCode} alt="FiiCode" />
              <p class="caption">FiiCode informatics competition</p>
            </div>
          </div>

        </div>
        <div className='box'>
          <img className='logo' src= {mironobotLogo} alt="Logo" />
          <p className='paragraph-text-2'>Volunteer in the robotics team of the high school. I took part in the regional stage of the competition.</p>
          <div class="polaroid polaroid2 orientation2">
              <img src= {robotics} alt="robotics" />
              <p class="caption">FTC regional competition </p>
            </div>
        </div>
      </div>
      <Footer/>
    </div>
  )
}

export default Volunteering;
