export async function getAvailablePlaces() {
  const response = await fetch("http://localhost:3000/places");
  const resData = await response.json();

  if (!response.ok) {
    throw new Error("Failed to fetch places, please try again later.");
  }

  return resData.places;
}

export async function updateSelectedPlaces(places) {
  const response = await fetch("http://localhost:3000/user-places", {
    method: "PUT",
    body: JSON.stringify({ places }),
    headers: {
      "Content-Type": "application/json",
    },
  });
  const resData = await response.json();

  if (!response.ok) {
    throw new Error("Failed to update selected places.");
  }

  return resData.message;
}

export async function getUserPlaces() {
  const response = await fetch("http://localhost:3000/user-places");
  const resData = await response.json();

  if (!response.ok) {
    throw new Error("Fetch user places");
  }

  return resData.places;
}
