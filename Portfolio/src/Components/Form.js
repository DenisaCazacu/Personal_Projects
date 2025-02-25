import React from 'react'
import "./FormStyle.css"
import "./ButtonsStyle.css"
export const Form = () => {
  const onSubmit = async (event) => {
    event.preventDefault();
    const formData = new FormData(event.target);

    formData.append("access_key", "51ecfb9b-cb1d-4141-8373-18f2553a6ce7");

    const object = Object.fromEntries(formData);
    const json = JSON.stringify(object);

    const res = await fetch("https://api.web3forms.com/submit", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        Accept: "application/json"
      },
      body: json
    }).then((res) => res.json());

    if (res.success) {
      console.log("Success", res);
      event.target.reset();  
    }
  };
  return (
    <div class="form-container">
      <form class="form" onSubmit={onSubmit}>
        <div className="form-group-2">
            <div class="form-group">
              <label>Name</label>
              <input type="text" id="name" name="name" required="" />
            </div>
            <div class="form-group">
              <label>Subject</label>
              <input type="text" id="subject" name="subject" required="" />
            </div>
        </div>
        <div class="form-group"> 
          <label for="email">Email</label>
          <input type="email" id="email" name="email" required="" />
        </div>
        <div class="form-group">
          <label for="textarea">How Can We Help You?</label>
          <textarea name="textarea" id="textarea" rows="10" cols="50" required="" placeholder="Your message"></textarea>
        </div>
        <button class="button" type="submit" >SUBMIT</button>
      </form>
    </div>
  )
}

export default Form;
