const mongoose = require('mongoose');

const measurementSchema = new mongoose.Schema({
  id: {
    type: String,
    required: true
  },
  key: {
    type: String,
    required: true
  },
  temperature: {
    type: Number,
    required: true
  },
  humidity: {
    type: Number,
    required: true
  },
  pressure: {
    type: Number,
    required: true
  }
});

const Measurement = mongoose.model('Measurement', measurementSchema);

module.exports = Measurement;
