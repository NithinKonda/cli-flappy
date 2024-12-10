import React from "react";
import SignIn from "./components/SignIn";
import ClickCounter from "./components/ClickCounter";

export default function Home() {
  return (
    <div>
      <SignIn />
      <ClickCounter />
    </div>
  );
}
