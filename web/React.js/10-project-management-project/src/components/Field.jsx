export default function Field({ label, textarea, ...props }) {
  const classNames =
    "w-full p-1 border-b-2 rounded-sm border-stone-300 bg-stone-200 text-stone-600 focus:outline-none focus:border-stone-600";
  return (
    <p className="flex flex-col gap-1 my-4">
      <label className="text-sm text-stone-500 uppercase font-bold">
        {label}
      </label>
      {textarea ? (
        <textarea className={classNames} {...props} />
      ) : (
        <input className={classNames} {...props} />
      )}
    </p>
  );
}
