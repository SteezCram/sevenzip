const assert = require('assert');
const fs = require('fs');
const path = require('path');
const sevenZip = require('../index');




// TO CHANGE TO FIT WITH YOUR TEST !!!
const DIR_TO_COMPRESS = __dirname;
const DESTINATION = path.join(__dirname, 'data');


describe('compression test', function()
{
    // Big timeout to prevent mocha to fail
    this.timeout(100000);

    it('7z compress', async function() {
        assert.equal(await sevenZip.compress('7z', {dir: DIR_TO_COMPRESS, destination: DESTINATION}), undefined, undefined);
    });

    it('tar compress', async function() {
        assert.equal(await sevenZip.compress('tar', {dir: DIR_TO_COMPRESS, destination: DESTINATION}), undefined, undefined);
    });

    it('zip compress', async function() {
        assert.equal(await sevenZip.compress('zip', {dir: DIR_TO_COMPRESS, destination: DESTINATION}), undefined, undefined);
    });
});

describe('extract test', function()
{
    // Big timeout to prevent mocha to fail
    this.timeout(100000);

    it('7z extract', async function() {
        assert.equal(await sevenZip.extract('7z', {archive: path.join(DESTINATION, `${path.basename(DIR_TO_COMPRESS)}.7z`), destination: DESTINATION}), undefined, undefined);

        // Delete the file to save space on the disk
        fs.unlinkSync(path.join(DESTINATION, `${path.basename(DIR_TO_COMPRESS)}.7z`));
    });

    it('tar extract', async function() {
        assert.equal(await sevenZip.extract('tar', {archive: path.join(DESTINATION, `${path.basename(DIR_TO_COMPRESS)}.tar`), destination: DESTINATION}), undefined, undefined);

        // Delete the file to save space on the disk
        fs.unlinkSync(path.join(DESTINATION, `${path.basename(DIR_TO_COMPRESS)}.tar`));
    });

    it('zip extract', async function() {
        assert.equal(await sevenZip.extract('zip', {archive: path.join(DESTINATION, `${path.basename(DIR_TO_COMPRESS)}.zip`), destination: DESTINATION}), undefined, undefined);

        // Delete the file to save space on the disk
        fs.unlinkSync(path.join(DESTINATION, `${path.basename(DIR_TO_COMPRESS)}.zip`));
    });
});