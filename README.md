# 1cbyc distributed file system

I was trying to build a Distributed File System (DFS) that allows multiple clients to store, retrieve, and manage files across several machines, emulating a simple cloud storage system

**This is how this system would work:**
* Upload, download, and delete files from a distributed network.
* Ensure data consistency across all nodes using concepts like replication and fault tolerance.
* Implement a working file indexing system to manage file locations in the distributed system I use.


### what am i even doing?

1. i've made sure it all works on a basic level, so now i want to add a file upload functionality to allow clients to send files to the server. that way, when the server gets it, it will store the file and acknowledge receipt.



## Release Instructions

This is how I make my releases

```bash
git tag -a v1.0.0 -m "Release version 1.0.0"
```
- with message

```bash
git tag -a v1.0.0
```
- without message

```bash
git tag -a v1.1.0 -m "Minor updates and bug fixes"
git push origin v1.1.0
```
**and how i name releases:**
minor updates - `+0.0.1`
major updates - `+0.1.x`
full product revamp/repurposing - `+1.x.x`