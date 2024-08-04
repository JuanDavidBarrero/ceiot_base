const mongoose = require('mongoose');

const deviceSchema = new mongoose.Schema({
  id: {
    type: String,
    required: true,
    unique: true
  },
  name: {
    type: String,
    required: true
  },
  key: {
    type: String,
    required: true
  }
});

const Device = mongoose.model('Device', deviceSchema);

module.exports = Device;
