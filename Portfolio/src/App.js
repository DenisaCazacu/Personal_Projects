import React from "react";
import {Route, Routes} from "react-router-dom"; 
import "./index.css";
import Home from "./routes/Home"
import Projects from "./routes/Projects"
import Volunteering from "./routes/Volunteering"
import AboutMe from "./routes/AboutMe"
import Contact from "./routes/Contact"

function App() {
  return (
    <>
    <Routes>
      <Route path="/" element={<Home />} />
      <Route path="/projects" element={<Projects />} />
      <Route path="/volunteering" element={<Volunteering />} />
      <Route path="/about" element={<AboutMe />} />
      <Route path="/contact" element={<Contact />} />
    </Routes>
    </>
  );
}

export default App;
