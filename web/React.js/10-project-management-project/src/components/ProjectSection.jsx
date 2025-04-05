import { useRef } from "react";

export function ProjectSection({ project, onAdd, onRemove }) {
  return (
    <section>
      <h2>{project.title}</h2>
      <p>{project.dueDate}</p>
      <p>{project.description}</p>
      <Tasks project={project} onAdd={onAdd} onRemove={onRemove} />
    </section>
  );
}

function Tasks({ project, onAdd, onRemove }) {
  const taskRef = useRef();

  return (
    <div>
      <h3>Tasks</h3>
      <div>
        <input
          ref={taskRef}
          type="text"
          className="bg-stone-200 focus:outline-blue-500"
        />
        <button
          onClick={() => {
            onAdd({ id: project.tasks.length, title: taskRef.current.value });
            taskRef.current.value = "";
          }}
          className="text-neutral-400 bg-stone-600"
        >
          Add Task
        </button>
      </div>
      <ul>
        {project.tasks.map((task, i) => (
          <Task key={`task-${i}`} title={task.title} onClear={onRemove}></Task>
        ))}
      </ul>
    </div>
  );
}

function Task({ id, title, onClear }) {
  return (
    <li>
      <span>{title}</span>
      <button onClick={() => onClear(id)}>Clear</button>
    </li>
  );
}
