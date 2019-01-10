When Using Code
===

# A couple of notes for the code
### 1. For working on laptops and testing usings sims
  - [WebCam]Change the cs::UsbCamera cam{"USBCam", 0}; to cs::UsbCamera cam{"USBCam", 1};
  for some laptops, if it doesn't work. use the original 0


### 2. For displaying
  - [Displaying](you'll need to swap out the ims's for output.PutFrame)
  because that doesn't run on tinkerboards,
  - You will need to grab the .img file from the zip. The .img file is what you give to etcher.
  - After complete, insert the microSD card and startup the coprocessor. It may take a minute or two.

### 3. Comment out wait keys
  - [WaitKeys](The Wait keys are required for simulations because thecode runs too fast. But on a tinkerboard you'll need to take that out... because a keyboard doesn't exist)