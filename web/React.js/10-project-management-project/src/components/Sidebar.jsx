import Button from "./Button.jsx";

export function Sidebar({ projects, onClick, onAdd }) {
  return (
    <aside className="w-1/3 px-8 py-16 bg-stone-900 text-stone-50 md:w-72 rounded-r-xl">
      <h2 className="mb-8 font-bold uppercase md:text-xl text-stone-200">
        Your Projects
      </h2>
      <div>
        <Button>+ Add Project</Button>
      </div>
      <ul role="list" className="grid grid-flow-row auto-rows-max gap-2">
        {projects.map((project, i) => (
          <li
            className="text-neutral-400 bg-stone-700 pl-2 py-1 pr-12  active:bg-stone-500 active:text-neutral-200"
            key={`project-${i}`}
            onClick={() => onClick({ ...project })}
          >
            {project.title}
          </li>
        ))}
      </ul>
    </aside>
  );
}
