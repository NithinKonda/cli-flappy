"use client"
import React, { useEffect, useState } from "react";
import { auth, db } from "@/firebase-config";
import { doc,getDoc, updateDoc, setDoc } from "firebase/firestore";

const ClickCounter = () => {
  const [clicks, setClicks] = useState(0);
  const [user, setUser] = useState(null);

  useEffect(() => {
    const unsubscribe = auth.onAuthStateChanged(async (currentUser) => {
      if (currentUser) {
        setUser(currentUser);
        const userDoc = doc(db, "users", currentUser.uid);
        const userData = await getDoc(userDoc);

        if (userData.exists()) {
          setClicks(userData.data().clicks || 0);
        } else {
          await setDoc(userDoc, { clicks: 0 });
        }
      }
    });

    return unsubscribe;
  }, []);

  const handleButtonClick = async () => {
    if (user) {
      const userDoc = doc(db, "users", user.uid);
      const newClickCount = clicks + 1;

      setClicks(newClickCount);
      await updateDoc(userDoc, { clicks: newClickCount });
    }
  };

  return (
    <div style={{ textAlign: "center", marginTop: "50px" }}>
      <h1>Hi {user?.displayName || "User"}</h1>
      <p>Click Count: {clicks}</p>
      <button onClick={handleButtonClick}>Click Me!</button>
    </div>
  );
};

export default ClickCounter;
