import React from 'react'
import Navbar from "../Components/Navbar"
import HelloSection from "../Components/HelloSection"
import IntroductionSection from "../Components/IntroductionSection"
import Footer from "../Components/Footer"

export const Home = () => {
  return (
    <div>
      <Navbar/>
      <HelloSection/>
      <IntroductionSection />
      <Footer />
    </div>
  )
}
export default Home;