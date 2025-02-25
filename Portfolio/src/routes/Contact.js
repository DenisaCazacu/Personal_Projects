import React from 'react'
import Footer from "./../Components/Footer.js"
import Navbar from "./../Components/Navbar.js"
import Form from "./../Components/Form.js"
import "./ContactStyle.css"

export const Contact = () => {
  return (
    <div className='contact-section'>
      <Navbar/>
      <Form/>
      <Footer/>
    </div>
  )
}

export default Contact;