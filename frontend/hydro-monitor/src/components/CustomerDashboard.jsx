import React, { useEffect, useState } from "react";
import { db, ref, onValue } from "../firebase";
import "bootstrap/dist/css/bootstrap.min.css";

function CustomerDashboard() {
  const [liters, setLiters] = useState(0);
  const [units, setUnits] = useState(0);
  const [pressure, setPressure] = useState("");
  const [pumpStatus, setPumpStatus] = useState("");

  useEffect(() => {
    const monitorRef = ref(db, "water_monitor");
    const pumpRef = ref(db, "pump_house");

    onValue(monitorRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        setLiters(data.liters_used || 0);
        setUnits(data.units_used || 0);
        setPressure(data.pressure_status || "normal");
      }
    });

    onValue(pumpRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        setPumpStatus(data.relay_status || "off");
      }
    });
  }, []);

  return (
    <div className="container mt-5 p-4 shadow rounded bg-light">
      <h2 className="mb-4 text-center">💧 Water Consumption Dashboard</h2>
      <div className="row text-center">
        <div className="col-md-6 mb-3">
          <div className="card p-3 shadow">
            <h4>Liters Used</h4>
            <p>{liters.toFixed(2)} L</p>
          </div>
        </div>
        <div className="col-md-6 mb-3">
          <div className="card p-3 shadow">
            <h4>Units Used</h4>
            <p>{units.toFixed(3)} units</p>
          </div>
        </div>
        <div className="col-md-6 mb-3">
          <div className="card p-3 shadow">
            <h4>Pressure</h4>
            <p className={pressure === "low" ? "text-danger" : "text-success"}>
              {pressure}
            </p>
          </div>
        </div>
        <div className="col-md-6 mb-3">
          <div className="card p-3 shadow">
            <h4>Pump Status</h4>
            <p>{pumpStatus}</p>
          </div>
        </div>
      </div>
    </div>
  );
}

export default CustomerDashboard;
