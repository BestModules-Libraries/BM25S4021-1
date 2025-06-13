<div align=center>
<img src="https://github.com/BestModules-Libraries/img/blob/main/BM25S4021-1_BME63K402_BME63K402A_V1.0.png" width="562" height="480">

</div>

BM25S4021-1
===========================================================

The BM25S4021-1 is a TDS water quality detection module from BestModules, which uses the UART communication method. This document provides the description of the BM25S4021-1 Arduino Lib functions and how to install the Arduino Lib. The example uses the BM25S4021-1 module to demonstrate functions such as reading TDS values and setting module ID.

#### Applicable types:
<div align=center>

|Part No.   |Description                             |
|:---------:|:--------------------------------------:|
|BM25S4021-1 |TDS water quality detection module|
|BME63K402   |adapter line and BM25S4021-1|
|BME63K402A  |adapter line and BM25S4021-1|

</div>

This library can be installed via the Arduino Library manager. Search for **BM25S4021-1**.

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE.
* **library.properties** - General library properties for the Arduino package manager.

Documentation
-------------------

* **[BM25S4021-1 Arduino Library Description](https://www.bestmodulescorp.com/bm25s4021-1.html#tab-product2)** - Arduino Library Description.

Version History
-------------------

* **V1.0.1**  
&emsp;&emsp;- Initial public release.  
* **V1.0.2**  
&emsp;&emsp;- Change the function name setWarmValue(ID,channel) to setAlarmValue(ID,channel).  
&emsp;&emsp;- Improve example:setAlarmValue.  
&emsp;&emsp;- Change the function name readTemp(ID,channel) to readTemperature(ID,channel).  
&emsp;&emsp;- Add function:selectModule(ID),readTDS(channel),readTemperature(channel).  
* **V1.0.3**  
&emsp;&emsp;- Change the BMduino module model name from BME63M402/BME63M402A to BME63K402/BME63K402A. 
License Information
-------------------

This product is _**open source**_!

Please review the LICENSE.md file for license information.

If you have any questions or concerns on licensing, please contact technical support on our BEST MODULES CORP. Email:service@bestmodulescorp.com.

Distributed as-is; no warranty is given.

BEST MODULES CORP. 
