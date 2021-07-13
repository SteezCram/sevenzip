const assert = require('assert');
const fs = require('fs');
const sevenZip = require('../index');




// TO CHANGE TO FIT WITH YOUR TEST !!!
const DIR_TO_COMPRESS = 'C:\\Users\\tcroi\\Desktop\\DCSB-4.0.0.9';
const DESTINATION = 'C:\\Users\\tcroi\\Desktop\\';


describe('compression test', function()
{
    // Big timeout to prevent mocha to fail
    this.timeout(100000);

    it('7z compress', async function() {
        assert.equal(await sevenZip.compress("7z", {dir: DIR_TO_COMPRESS, destination: DESTINATION}), undefined);
    });

    it('tar compress', async function() {
        assert.equal(await sevenZip.compress("tar", {dir: DIR_TO_COMPRESS, destination: DESTINATION}), undefined);
    });

    it('wim compress', async function() {
        assert.equal(await sevenZip.compress("wim", {dir: DIR_TO_COMPRESS, destination: DESTINATION}), undefined);
    });

    it('zip compress', async function() {
        assert.equal(await sevenZip.compress("zip", {dir: DIR_TO_COMPRESS, destination: DESTINATION}), undefined);
    });
});

describe('extract test', function()
{
    // Big timeout to prevent mocha to fail
    this.timeout(100000);

    it('7z extract', async function() {
        assert.equal(await sevenZip.extract("7z", {archive: `${DIR_TO_COMPRESS}.7z`, destination: DESTINATION}), undefined);

        // Delete the file to save space on the disk
        fs.unlinkSync(`${DIR_TO_COMPRESS}.7z`);
    });

    it('tar extract', async function() {
        assert.equal(await sevenZip.extract("tar", {archive: `${DIR_TO_COMPRESS}.tar`, destination: DESTINATION}), undefined);

        // Delete the file to save space on the disk
        fs.unlinkSync(`${DIR_TO_COMPRESS}.tar`);
    });

    it('wim extract', async function() {
        assert.equal(await sevenZip.extract("wim", {archive: `${DIR_TO_COMPRESS}.wim`, destination: DESTINATION}), undefined);

        // Delete the file to save space on the disk
        fs.unlinkSync(`${DIR_TO_COMPRESS}.wim`);
    });

    it('zip extract', async function() {
        assert.equal(await sevenZip.extract("zip", {archive: `${DIR_TO_COMPRESS}.zip`, destination: DESTINATION}), undefined);

        // Delete the file to save space on the disk
        fs.unlinkSync(`${DIR_TO_COMPRESS}.zip`);
    });
});