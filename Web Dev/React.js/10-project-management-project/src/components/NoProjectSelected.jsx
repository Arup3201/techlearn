import NoProjectImage from "../assets/no-projects.png";
import Button from "./Button.jsx";

export default function NoProjectSelected({ onAdd }) {
  return (
    <div className="mt-24 text-center w-2/3">
      <img
        src={NoProjectImage}
        alt="An empty tasks list"
        className="h-16 w-16 object-contain mx-auto"
      />
      <h2 className="text-xl text-stone-500 my-4 font-bold">
        No project selected
      </h2>
      <p className="text-stone-400 mb-4">
        Select a project or get started with a new one
      </p>
      <p className="mt-8">
        <Button onClick={onAdd}>Create new project</Button>
      </p>
    </div>
  );
}
