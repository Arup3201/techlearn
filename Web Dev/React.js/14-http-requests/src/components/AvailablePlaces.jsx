import { useState } from "react";
import Places from "./Places.jsx";
import ErrorMsg from "./Error.jsx";
import { useEffect } from "react";
import { sortPlacesByDistance } from "../loc.js";
import { getAvailablePlaces } from "../http.js";

export default function AvailablePlaces({ onSelectPlace }) {
  const [availablePlaces, setAvailablePlaces] = useState([]);
  const [isFetching, setIsFetching] = useState(false);
  const [error, setError] = useState();

  useEffect(() => {
    async function getPlaces() {
      setIsFetching(true);

      try {
        const places = await getAvailablePlaces();

        navigator.geolocation.getCurrentPosition((position) => {
          const sortedPlaces = sortPlacesByDistance(
            places,
            position.coords.latitude,
            position.coords.longitude
          );
          setAvailablePlaces(sortedPlaces);
          setIsFetching(false);
        });
      } catch (error) {
        setError(error);
        setIsFetching(false);
      }
    }

    getPlaces();
  }, []);

  if (error) {
    return (
      <ErrorMsg
        title="An Error Occured When Fetching"
        message={error.message}
      />
    );
  }

  return (
    <Places
      title="Available Places"
      places={availablePlaces}
      fallbackText="No places available."
      loadingText="Loading available places..."
      loading={isFetching}
      onSelectPlace={onSelectPlace}
    />
  );
}
