const sevenZip = require('../index');




sevenZip.compress("7z", {dir: 'C:\\Users\\tcroi\\Desktop\\DCSB-4.0.0.9', destination: 'C:\\Users\\tcroi\\Desktop\\', level: sevenZip.CompressionLevel.ULTRA}, (error) => {
    if (error) throw error;
});