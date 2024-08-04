const express = require("express");
const bodyParser = require("body-parser");
const mongoose = require('mongoose');
const Device = require('./models/devices');
const Measurement = require('./models/measurement');


const render = require("./render.js");

let database = null;

async function starDB() {
    try {
        const uri = 'mongodb://192.168.1.46:27017/iot';

        await mongoose.connect(uri, {
        });

        database = mongoose.connection.db;
        console.log('Base de datos persistente online conectada');
    } catch (error) {
        console.error('Error en la base de datos:', error);
        throw new Error('Error en la base de datos');
    }
}

// API Server

const app = express();

app.use(bodyParser.urlencoded({ extended: false }));

app.use(express.json());

app.use(express.static('spa/static'));

const PORT = 8080;


app.post('/measurement', async function (req, res) {
    try {
        const { id, key, t, h, p } = req.body;

        if (!id || !key || t === undefined || h === undefined || p === undefined) {
            return res.status(400).send("Missing required fields.");
        }

        const device = await Device.findOne({ id: id });

        if (!device) {
            console.log("Device not found.");
            return res.status(404).send("Device not found.");
        }

        const newMeasurement = new Measurement({
            id: id,
            temperature: t,
            humidity: h,
            pressure: p,  
            key: key
        });

        const savedMeasurement = await newMeasurement.save();

        res.send("Received measurement with ID: " + savedMeasurement._id);
    } catch (error) {
        console.error("Error processing measurement:", error);
        res.status(500).send("Error processing measurement.");
    }
});


app.post('/device', async function (req, res) {
    try {
        const { id, n: name, k: key } = req.body;

        if (!id || !name || !key) {
            return res.status(400).send("Error: All fields (id, name, key) are required.");
        }

        const macRegex = /^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$/;
        if (!macRegex.test(id)) {
            return res.status(400).send("Error: Invalid MAC address format.");
        }

        const existingDevice = await Device.findOne({ id });
        if (existingDevice) {
            console.log("Error: Device ID already exists.");
            
            return res.status(400).send("Error: Device ID already exists.");
        }

        console.log("device id    : " + id + " name        : " + name + " key         : " + key);

        const newDevice = new Device({
            id,
            name,
            key
        });
        await newDevice.save();

        res.send("Received new device");
    } catch (error) {
        console.error("Error processing request:", error);
        res.status(500).send("Error processing request.");
    }
});


app.get('/web/device', async function (req, res) {
    try {
        // Obtener todos los documentos de la colección `devices` de la base de datos
        const devices = await Device.find({});

        // Transformar los documentos para renombrar 'id' a 'device_id'
        const deviceRows = devices.map(device => {
            // Cambiar el campo 'id' por 'device_id' en el HTML
            return `<tr>
                        <td><a href="/web/device/${device.id}">${device.id}</a></td>
                        <td>${device.name}</td>
                        <td>${device.key}</td>
                    </tr>`;
        }).join('');

        // Enviar la respuesta HTML
        res.send(`
            <html>
                <head><title>Sensores</title></head>
                <body>
                    <table border="1">
                        <tr><th>device_id</th><th>name</th><th>key</th></tr>
                        ${deviceRows}
                    </table>
                </body>
            </html>
        `);
    } catch (error) {
        console.error("Error retrieving devices:", error);
        res.status(500).send("Error retrieving devices.");
    }
});


app.get('/web/device/:id', async function (req, res) {
    try {

    
        const device = await Device.findOne({ id: req.params.id });

        if (!device) {
            return res.status(404).send("Device not found.");
        }

        const template = `
            <html>
                <head><title>Sensor ${device.name}</title></head>
                <body>
                    <h1>${device.name}</h1>
                    id  : ${device.id}<br/>
                    Key : ${device.key}
                </body>
            </html>
        `;

        res.send(template);
    } catch (error) {
        console.error("Error retrieving device:", error);
        res.status(500).send("Error retrieving device.");
    }
});


app.get('/term/device/:id', function (req, res) {
    var red = "\33[31m";
    var green = "\33[32m";
    var blue = "\33[33m";
    var reset = "\33[0m";
    var template = "Device name " + red + "   {{name}}" + reset + "\n" +
        "       id   " + green + "       {{ id }} " + reset + "\n" +
        "       key  " + blue + "  {{ key }}" + reset + "\n";
    var device = db.public.many("SELECT * FROM devices WHERE device_id = '" + req.params.id + "'");
    console.log(device);
    res.send(render(template, { id: device[0].device_id, key: device[0].key, name: device[0].name }));
});

app.get('/measurement', async (req, res) => {
    try {
        const measurements = await Measurement.find({}, '-_id -__v');

        if (measurements.length === 0) {
            return res.status(404).send("No measurements found.");
        }

        res.json(measurements);
    } catch (error) {
        console.error("Error retrieving measurements:", error);
        res.status(500).send("Error retrieving measurements.");
    }
});

app.get('/device', async function (req, res) {
    try {
        
        const devices = await Device.find({});

        if (devices.length === 0) {
            return res.status(404).send("No devices found.");
        }

        const transformedDevices = devices.map(device => {
            return {
                device_id: device.id,  
                name: device.name,
                key: device.key
            };
        });

        res.json(transformedDevices);
    } catch (error) {
        console.error("Error retrieving devices:", error);
        res.status(500).send("Error retrieving devices.");
    }
});


starDB().then(async () => {

    const addAdminEndpoint = require("./admin.js");
    addAdminEndpoint(app, render);



    app.listen(PORT, () => {
        console.log(`Listening at ${PORT}`);
    });
});
