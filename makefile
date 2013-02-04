# Main compile command. -I flag configurates path for include files(for boost in this case), -L for libraries(for easybmp). 
# Additional linker options is required for adding new path string to global PATH. 
# -lEasyBMP for dynamic linking with subj
nvcc main.cpp -lEasyBMP -o imgsmth -I /home/skondratov/workspace/ -L /home/skondratov/workspace/easybmp/ --linker-options '-rpath=/home/skondratov/workspace/easybmp/'

#EasyBMP lib compilation string
g++ -O3 -pipe -fomit-frame-pointer -funroll-all-loops -shared -fPIC EasyBMP.cpp -o libEasyBMP.so

