# Loose ends

Loose ends is a tools to drop the base at the read ends (both start and end) from a samtools pileup file. 

It can be used like that:

`samtools mpileup -aa -A -d 0 -Q 0 <input.bam> | loose_ends`

# Build

```
make
```

# Test

```
cat test/input.pileup | ./loose_ends
```
