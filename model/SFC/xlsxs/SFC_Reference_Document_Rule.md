# Document Rules

- This document consists of a index and the list of the sfc-modules as follows:
  * ```
    Index
    	(contents of Index)
    
    <sfc-module-1>
    	(contents of sfc-module-1)
    
    <sfc-module-2>
    	(contents of sfc-module-2)
    
    <sfc-module-3>
    	(contents of sfc-module-3)
    
    ...
    ```

- Document-wide font color rules:
  - <b><font color=607D3D>Dark Green</font></b>: Modified
  - <b><font color=03AC13>Green</font></b>: New
  - <b><del><font color=FF0000>Red+Strikethrough</font></del></b>: Removed
  <br>

## [Rule] Index

- The contents of index are the lists of the sfc-module name.
- The sfc-module name is written with the corresponding spec-type-number.
- Each index can be selected. When selected, it moves to the page of the corresponding sfc-module.
- Notation:
  * ```
    Index
        <spec-type>
            [<spec-type-number>]<sfc-module-1>
            [<spec-type-number>]<sfc-module-2>
            [<spec-type-number>]<sfc-module-3>
            ...
    ```
    - spec-type:
      - AD, AV, CD, CH, CS, EC, HD, PT, ETC
    - spec-type-number:
      - AD080, AD090, ..., PT500
    - sfc-module:
      - ADAS_Driving_New, ADAS_Parking_NEW, ..., EV_Accel_Boost_Mode
      <br>

## [Rule] SFC Module
- In each sfc-module content, the module information(module-version and module-description), input-signal list, and output-signal list are described.
- Notation:
  * ```
    <sfc-module>
    	<module-version>
    	<module-description>
        Input
          <input-signal-1>
          <input-signal-2>
          <input-signal-3>
          ...
    
        Output
          <output-signal-1>
          <output-signal-2>
          <output-signal-3>
          ...
    ```

### Input
- Notation:
  * ```
    <input-signal>
    	[Signal Name]
    		- <signal-name>
    ```
    - Each input-signal is the input node address of the signal that the sfc module receives from outside.
    - Depending on the signal type, each of these can be described with some signal names as follows:
      - CAN Signal
        - Address: Vehicle.[spec-type].XXX.XXX
        - In the case of the CAN signal, the signal-name used for each Power-Train(ICV, EV, HEV, PHEV, FCEV, EREV) is defined.
      - SFC Inter Signal
        - Address: Vehicle.System.XXX.XXX
        - In the case of the inter signal, the signal-name used by the inter signal is defined.
      - SFC Private Signal
        - Address: SFC.Private.XXX.XXX
        - In case of the private signal, the signal-name is not defined.

### Output
- Notation:
  * ```
    <output-signal>
    	<ouput-signal-description>				[Group:<group-name>(<gruop-count>/<gruop-total-count>)]
    	DataType: <data-type>
    	Means of Value
    		<enum-value>
    	Implementation:
    		<enum-value-implementation>
    ```
    - output-signal
      - Each output-signal is the output node address sent out from the sfc module:
        - SFC.[module-name].XXX.[feature-name].XXX
    - output-signal-description
      - This means the description of the output node.
      - In case of output-signal related to event ID or sound ID,
        Event ID List or Sound ID List is additionally defined in ouput-signal-description.
    - data-type
      - This means the data type of the output signal.
    - Group
      - group-name
        - For output signals that have dependency, the group-name is defined.
      - group-count / group-total-count
        - It means what number(group-count) it is among the total number(group-total-count) of the same group-name.
    - Means of Value / Implementation
      - If the output signal uses enum values, the enum value info and the actual implementation sample are described.


---

<br>

