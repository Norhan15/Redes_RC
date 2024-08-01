const express = require('express');
const mysql = require('mysql2');
const bodyParser = require('body-parser');
require('dotenv').config();


const app = express();
const port = 3000;

// Configurar body-parser para manejar JSON
app.use(bodyParser.json());

// Configurar la conexión a MySQL
const db = mysql.createConnection({
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE,
    host: process.env.DB_HOST
});

db.connect(err => {
    if (err) {
        console.error('Error conectando a la base de datos:', err.stack);
        return;
    }
    console.log('Conectado a la base de datos MySQL');
});

app.post('/api/redes', (req, res) => {
    const { ssid, mac_address, rssi, encryption_type } = req.body;

    const query = `
        INSERT INTO redes (ssid, mac_address, rssi, encryption_type)
        VALUES (?, ?, ?, ?)
        ON DUPLICATE KEY UPDATE rssi = ?, last_seen = NOW();
    `;

    db.query(query, [ssid, mac_address, rssi, encryption_type, rssi], (err, result) => {
        if (err) {
            console.error('Error ejecutando la consulta:', err.stack);
            res.status(500).send('Error en el servidor');
            return;
        }
        res.status(200).send('Datos guardados correctamente');
    });
});


app.listen(port, () => {
    console.log(`API escuchando en http://localhost:${port}`);
});
