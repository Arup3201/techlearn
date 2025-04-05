export default function UserInput({ onChange, userInputs }) {
  return (
    <section id="user-input">
      <div className="input-group">
        <p>
          <label htmlFor="initial_investment">{"Initial Investment"}</label>
          <input
            type="number"
            required
            value={userInputs.initialInvestment}
            onChange={(e) => onChange("initialInvestment", e.target.value)}
          ></input>
        </p>
        <p>
          <label htmlFor="annual_investment">{"Annual Investment"}</label>
          <input
            type="number"
            required
            value={userInputs.annualInvestment}
            onChange={(e) => onChange("annualInvestment", e.target.value)}
          ></input>
        </p>
      </div>
      <div className="input-group">
        <p>
          <label htmlFor="expected_return">{"Expected Return"}</label>
          <input
            type="number"
            required
            value={userInputs.expectedReturn}
            onChange={(e) => onChange("expectedReturn", e.target.value)}
          ></input>
        </p>
        <p>
          <label htmlFor="duration">{"Duration"}</label>
          <input
            type="number"
            required
            value={userInputs.duration}
            onChange={(e) => onChange("duration", e.target.value)}
          ></input>
        </p>
      </div>
    </section>
  );
}
