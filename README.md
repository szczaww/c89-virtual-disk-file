<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![Stargazers][stars-shield]][stars-url]
[![Unlicense License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)


<!-- ABOUT THE PROJECT -->
## About

File system manager written from scratch in C89 for use with Minix/Linux operating systems. Proof of concept that supports simple operations like:
- create & delete diskfiles
- write to & extract from
- visualize disk structure
- list files & info

Made as a part of Operating System course at Warsaw University of Technology.

## Installation

### Linux

1. Clone the repo
   ```
   git clone https://github.com/szczaww/c89-virtual-disk-file
   ```
2. Build the project
   ```
   make
   ```
3. Run a demo bash script (Optional) 
   ```
   make demo
   ```

### Minix

1. Clone the repo
   ```
   git clone https://github.com/szczaww/c89-virtual-disk-file
   ```
2. Transfer ./minix/usr contents into usr/ directory under your minix virtual machine
3. Build the project
   ```
   make
   ```
4. Run a demo bash script (Optional) 
   ```
   make demo
   ```

<!-- USAGE EXAMPLES -->
## Example usage

1. **Create a new disk**:  
   `./program -c mydisk 1000`  
   This creates a new disk named `mydisk` with a size of 1000 units.

2. **Add a file**:  
   `./program -a mydisk file.txt`  
   This adds `file.txt` to the `mydisk`.

3. **Remove a file**:  
   `./program -r mydisk file.txt`  
   This removes `file.txt` from `mydisk`.

4. **Extract a file**:  
   `./program -e mydisk file.txt`  
   This extracts `file.txt` from `mydisk`.

5. **Delete a disk**:  
   `./program -d mydisk`  
   This deletes the `mydisk` disk.

6. **Show inode bitmap**:  
   `./program -inm mydisk`  
   This shows the bitmap for inodes on the `mydisk`.

7. **Show block bitmap**:  
   `./program -dbm mydisk`  
   This shows the bitmap for blocks on the `mydisk`.

8. **List normal files**:  
   `./program -ls mydisk`  
   This lists all normal files on `mydisk`.

9. **List normal and hidden files**:  
   `./program -lsh mydisk`  
   This lists all normal and hidden files on `mydisk`.

10. **Print help message**:  
    `./program -h`  
    This prints the help message.


<!-- MARKDOWN LINKS & IMAGES -->
[stars-shield]: https://img.shields.io/github/stars/szczaww/c89-virtual-disk-file.svg?style=for-the-badge
[stars-url]: https://github.com/szczaww/c89-virtual-disk-file/stargazers
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: www.linkedin.com/in/kamil-szczawinski
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: www.linkedin.com/in/kamil-szczawinski

