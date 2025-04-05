import { useState } from "react";
import Header from "./components/Header";
import UserInput from "./components/UserInput";
import Result from "./components/Result";

const TABLE_HEADERS = [
  "Year",
  "Investment Value",
  "Interest(Year)",
  "Total Interest",
  "Invested Capital",
];

function App() {
  const [userInputs, setUserInputs] = useState({
    initialInvestment: 10000,
    annualInvestment: 1200,
    expectedReturn: 6,
    duration: 10,
  });

  const inputIsValid = userInputs.duration >= 1;

  function handleInputChange(inputName, newValue) {
    setUserInputs((prevValues) => ({
      ...prevValues,
      [inputName]: +newValue,
    }));
  }
  return (
    <>
      <Header />
      <UserInput onChange={handleInputChange} userInputs={userInputs} />
      {inputIsValid && <Result inputs={userInputs} />}
      {!inputIsValid && (
        <p className="center">Please enter a duration more than 0.</p>
      )}
    </>
  );
}

export default App;
