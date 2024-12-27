import { render, screen } from "@testing-library/react";
import Greeting from "./Greeting.jsx";

test("rendering hello world in greeting component", () => {
  // Arrange
  render(<Greeting />);

  // Act
  // ...

  // Assert
  const element = screen.getByText("Hello World!");
  expect(element).toBeInTheDocument();
});
