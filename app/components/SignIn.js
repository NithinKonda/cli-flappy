"use client"
import React, { useState, useEffect } from "react";
import { GoogleAuthProvider, signInWithPopup, sendEmailVerification, onAuthStateChanged } from "firebase/auth";
import { auth } from "@/firebase-config";

const SignIn = () => {
  const [user, setUser] = useState(null);

  useEffect(() => {
    const unsubscribe = onAuthStateChanged(auth, (currentUser) => {
      setUser(currentUser);
    });
    return unsubscribe;
  }, []);

  const handleGoogleSignIn = async () => {
    const provider = new GoogleAuthProvider();
    try {
      const result = await signInWithPopup(auth, provider);
      const user = result.user;

      // Send verification email if the email is not verified
      if (!user.emailVerified) {
        await sendEmailVerification(user);
        alert("Verification email sent! Please verify your email.");
      }
    } catch (error) {
      console.error(error.message);
    }
  };

  const handleSignOut = async () => {
    await auth.signOut();
    setUser(null);
  };

  return (
    <div style={{ textAlign: "center", marginTop: "50px" }}>
      {user ? (
        <>
          <h1>Hi, {user.displayName}</h1>
          <button onClick={handleSignOut}>Sign Out</button>
        </>
      ) : (
        <button onClick={handleGoogleSignIn}>Sign in with Google</button>
      )}
    </div>
  );
};

export default SignIn;
