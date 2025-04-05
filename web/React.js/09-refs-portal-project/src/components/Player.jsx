import { useState, useRef } from "react";

export default function Player() {
  const playerNameRef = useRef();

  const [playerName, setPlayerName] = useState();

  function handleSetName() {
    setPlayerName(playerNameRef.current.value);
    playerNameRef.current.value = "";
  }

  return (
    <section id="player">
      <h2>Welcome {playerName || "unknown entity"}</h2>
      <p>
        <input ref={playerNameRef} type="text" />
        <button onClick={handleSetName}>Set Name</button>
      </p>
    </section>
  );
}
