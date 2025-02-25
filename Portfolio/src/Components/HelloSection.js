import React from "react";
import "./HelloSectionStyle.css";
import CardImage from "../Images/HelloSectionBackground.jpg";
import ProfileImage from "../Images/profile-picture.jpg";
import DownloadButton from "./DownloadButton";

export const HelloSection = () => {
  return (
    <div className="hello-section">
      <div className="hello-section-container">
        <img className="hello-card-image" src={CardImage} alt={CardImage} />
        <div className="text-and-image">
          <div className="circular-container">
            <img className="profile-image" src={ProfileImage} alt={ProfileImage} />
          </div>
          <div className = "text">
            <h1 className="msg1">HELLO, I'M</h1>
            <p className="msg2">Denisa</p>
            <p className="msg3"> Computer Science Student</p>
          </div>
          <DownloadButton />
        </div>
      </div>
    </div>
  )
}

export default HelloSection;