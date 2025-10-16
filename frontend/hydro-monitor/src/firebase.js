// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue } from "firebase/database";

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyBYxUm866EoQQloADLINmiXZt_zeFHsW9Q",
  authDomain: "newhydro-cd451.firebaseapp.com",
  databaseURL: "https://newhydro-cd451-default-rtdb.firebaseio.com",
  projectId: "newhydro-cd451",
  storageBucket: "newhydro-cd451.firebasestorage.app",
  messagingSenderId: "798591405128",
  appId: "1:798591405128:web:567f3835572baf45b6dc7d",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// Initialize Realtime Database
const db = getDatabase(app);

// ✅ Export needed Firebase objects
export { db, ref, onValue };
