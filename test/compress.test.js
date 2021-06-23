const sevenZip = require('../index');




sevenZip.compress("tar", {dir: 'C:\\Users\\tcroi\\Desktop\\DCSB-4.0.0.9', destination: 'C:\\Users\\tcroi\\Desktop\\', level: sevenZip.CompressionLevel.ULTRA}, (error) => {
    console.error(error);
}, (data) => {
    console.log(data);
});