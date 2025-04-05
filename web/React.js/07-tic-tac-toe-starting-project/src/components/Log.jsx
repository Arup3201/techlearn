export default function Log({ turns }) {
  return (
    <ol id="log">
      {turns.map(({ square, player }) => (
        <li key={`${square.row}${square.col}`}>
          {"Player " +
            player +
            " selected " +
            square.row +
            " row, " +
            square.col +
            " col."}
        </li>
      ))}
    </ol>
  );
}
