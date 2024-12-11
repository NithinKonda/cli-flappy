"use client";

import React, { useEffect, useState } from "react";
import { auth,db } from "@/firebase-config";
import { signInWithPopup, GoogleAuthProvider, signOut } from "firebase/auth";
import Link from "next/link";

const HomePage = () => {
  const [user, setUser] = useState(null);

  useEffect(() => {
    const unsubscribe = auth.onAuthStateChanged((currentUser) => {
      setUser(currentUser);
    });

    return () => unsubscribe();
  }, []);

  const handleSignIn = async () => {
    const provider = new GoogleAuthProvider();
    try {
      await signInWithPopup(auth, provider);
    } catch (error) {
      console.error("Error signing in:", error.message);
    }
  };

  const handleSignOut = async () => {
    try {
      await signOut(auth);
    } catch (error) {
      console.error("Error signing out:", error.message);
    }
  };

  return (
    <div style={{ textAlign: "center", marginTop: "50px" }}>
      {!user ? (
        <button onClick={handleSignIn}>Sign In with Google</button>
      ) : (
        <div>
          <h1>Hi {user.displayName}</h1>
          <button onClick={handleSignOut}>Sign Out</button>
          <br />
          <Link href="/game">
            <a style={{ fontSize: "20px", color: "blue", textDecoration: "underline" }}>Game</a>
          </Link>
        </div>
      )}
    </div>
  );
};

export default HomePage;
