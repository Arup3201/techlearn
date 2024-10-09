import { forwardRef, useImperativeHandle, useRef } from "react";
import { createPortal } from "react-dom";

const ResultsModal = forwardRef(
  ({ targetTime, remainingTime, onReset }, ref) => {
    const dialog = useRef();

    const userLost = remainingTime <= 0;
    const formattedRemainingTime = (remainingTime / 1000).toFixed(2);
    const score = ((1 - remainingTime / (targetTime * 1000)) * 100).toFixed();

    useImperativeHandle(ref, () => ({
      open() {
        dialog.current.showModal();
      },
    }));

    return createPortal(
      <dialog ref={dialog} className="result-modal" onClose={onReset}>
        {userLost && <h2>You lost</h2>}
        {!userLost && <h2>Your score {score}</h2>}
        <p>
          Your target time was <strong>{targetTime} seconds.</strong>
        </p>
        <p>
          You stopped the timer with{" "}
          <strong>{formattedRemainingTime} seconds left.</strong>
        </p>
        <form method="dialog" onSubmit={onReset}>
          <button>Close</button>
        </form>
      </dialog>,
      document.getElementById("modal")
    );
  }
);

export default ResultsModal;
