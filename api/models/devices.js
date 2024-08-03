const { Schema, model } = require('mongoose');


const DeviceSchema = new Schema({
    MAC: {
        type: String,
        required: [true, 'La dirección MAC es obligatoria'],
        unique: true 
    },
    datos: [
        {
            pressure: {
                type: Number,
            },
            temp: {
                type: Number,
            },
            hum: {
                type: Number,
            }
        }
    ]
});


module.exports = model('Device', DeviceSchema);
