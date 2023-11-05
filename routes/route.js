const express = require('express');
const tempController = require('../controllers/tempController');
const router = express.Router();

router.get('/api/getLogs/:deviceID', tempController.getLogs);
router.post('/api/logValue/:deviceID/:value', tempController.logValue);
router.post('/api/logState/:deviceID/:state', tempController.logState);

module.exports = router;