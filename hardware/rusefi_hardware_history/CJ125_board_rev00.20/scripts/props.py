#!/usr/bin/python
# This script will update KICAD .sch files with component properties found in the property.sch file.
#It first creates a file list, then opens each .sch file renames the old file to .sch.old file, and
#then it recreates the orginal file. 

import commands,sys,os
currentline = []
fileNames = os.listdir("../")#get file list
#fileNames = ["../digi_protect_I3.sch","../digi_protect_I4.sch","../freeEMS_lib.lib"]#limit how many fileNames we look at for debug purposes
#fileNames = ["digi_protect_I3.sch"]#limit how many fileNames we look at for debug purposes

mytemplatefile=open("property.sch","r")#Open template file and create a search variable, and a replacement text file
for i in range (1,6): #Read the first several lines effectively skipping these lines
  mytemplatefile.readline()#Increment the readline pointer
searchvar = mytemplatefile.readline()#create the search variable
searchvar = searchvar[0:searchvar.find('" ')+2]#save the part we need to compare against
#mytemplatefile.readline()#toss one line of the template
templateText=mytemplatefile.read()#create the replacement text variable.
mytemplatefile.close()#not needed any more, so close it.

for fileName in fileNames:#rake through each file
  if fileName.endswith (".sch"):#make sure it's a schematic
    if not os.path.exists('..//'+fileName+'.old'):#check that this file hasn't been processed yet, debug code
      print "Processing "+fileName#let us know you are working
      os.rename('..//'+fileName, '..//'+fileName+'.old')#remane newfile, to .old file
      inFile = open('..//'+fileName+'.old',"r")
      outFile = open('..//'+fileName,'w')
      line = inFile.readline()
      while line!='':# Process input file one line at a time, and replace when required
        if line.startswith(searchvar):#is a component to be replaced?
          outFile.write(line)#copy identified line, such the component designator is the same as the orginal file
          line = inFile.readline()#read one line from orginal file
          #outFile.write(line)#copy one line after identified line, such the component designator is the same as the orginal file
          outFile.write(templateText)#copy new data to the output file
          while line != '' and not line.startswith('	'):#skip the old data from the orginal file.
            line = inFile.readline()
          outFile.write(line)#not a new replaced component, so just copy the string to the new file
        else:
          outFile.write(line)#not a new replaced component, so just copy the string to the new file
        line=inFile.readline()#incriment the line
      outFile.close()
      inFile.close()
      os.remove('..//'+fileName+'.old')#remove old file
    else:#debug code
      print "Already Processed "+fileName#debug code
