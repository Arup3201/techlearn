import { useState } from "react";
import Input from "./Input.jsx";
import {
  isEmail,
  isNotEmpty,
  hasMinLength,
  isEqualsToOtherValue,
} from "../util/validation.js";

export default function Login() {
  const [enteredValues, setEnteredValues] = useState({
    email: "",
    password: "",
  });
  // whether input fields are blurred after being edited
  const [didEdit, setDidEdit] = useState({
    email: false,
    password: false,
  });

  const emailIsInvalid =
    didEdit.email &&
    !isEmail(enteredValues.email) &&
    !isNotEmpty(enteredValues.email);
  const passwordIsInvalid =
    didEdit.password && !hasMinLength(enteredValues.password, 6);

  function handleFormSubmit(event) {
    event.preventDefault();

    console.log(enteredValues);
  }

  function handleInputChange(value, id) {
    setEnteredValues((prev) => ({ ...prev, [id]: value }));
    setDidEdit((prevEdit) => ({
      ...prevEdit,
      [id]: false,
    }));
  }

  function handleInputBlur(id) {
    setDidEdit((prevEdit) => ({
      ...prevEdit,
      [id]: true,
    }));
  }

  return (
    <form onSubmit={handleFormSubmit}>
      <h2>Login</h2>

      <div className="control-row">
        <Input
          label="Email"
          id="email"
          type="email"
          name="email"
          value={enteredValues.email}
          onBlur={() => handleInputBlur("email")}
          onChange={(e) => handleInputChange(e.target.value, "email")}
          error={emailIsInvalid && "Please enter a valid email."}
        />

        <Input
          label="Password"
          id="password"
          type="password"
          name="password"
          value={enteredValues.password}
          onBlur={() => handleInputBlur("password")}
          onChange={(e) => handleInputChange(e.target.value, "password")}
          error={passwordIsInvalid && "Please enter a valid password."}
        />
      </div>

      <p className="form-actions">
        <button className="button button-flat">Reset</button>
        <button className="button">Login</button>
      </p>
    </form>
  );
}
