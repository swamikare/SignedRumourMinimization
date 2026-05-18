import subprocess
import sys
import os
#opening the file in read mode
my_file = open(sys.argv[1], "r")

#reading the file
data = my_file.read()

# replacing end splitting the text 
# when newline ('\n') is seen.
filelist = data.split("\n")
#itdtseed = [["randitdt", "randitdt", "randseed"], ["0.5", "0.3", "maxdegree"], ["0.1", "0.3", "maxdegree"]]
itdtseed = [["0.5", "0.3", "maxdegree"], ["0.1", "0.3", "maxdegree"]]
i = 0
#print("len",len(filelist))

while(i < len(filelist)-1):
    for idtype in itdtseed:        
        SRsize = ["3", "5", "10"]
        for rsc in SRsize:
            #TSsize = ["3", "5", "10"]
            for tsc in SRsize:                                                             
                noiter = 1
                #if(idtype[0] == "randitdt"):
                #    noiter = 1                    
                runcmd = "./a.out "+ filelist[i]+" "+rsc+" "+tsc+" "+idtype[0]+" "+idtype[1]+" "+idtype[2]+" runall "+ filelist[i+1]
                runcmd = runcmd + " "+ "50 100 2000"
                f1size = [str(round(x * 0.1, 1)) for x in range(1, 11)]
                for f1 in f1size:
                    for f2 in f1size:
                        final_cmd = f"{runcmd} {f1} {f2}"
                        #for k in range(noiter):
                        #print(final_cmd)
                        os.system(final_cmd)              
    i = i + 2
