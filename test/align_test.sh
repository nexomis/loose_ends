echo ">ref" > ref.fa
echo "ccccaaaagagttttatcgcttccatgacgcagaagttaacactttcggatatttctgatgagtcgaa" >> ref.fa

for i in $(seq 1 10) ; do  echo -E "@${i}";  echo "ccccaaaagagttttatcgcttccatgacgcagaagttaacactttcggat" ; echo "+" ; echo '===================================================' ; done > r1.fq
for i in $(seq 11 15); do echo -E "@${i}"; echo "ccccaaaagagttttacgcttccatgacgcagaagttaacactttcggat" ; echo "+" ; echo '==================================================' ; done >> r1.fq
for i in $(seq 16 20); do echo -E "@${i}"; echo "ccccaaaagagttttacgcttccatgacgcagaagttaacactttcggat" ; echo "+" ; echo '==================================================' ; done >> r1.fq
for i in $(seq 21 25) ; do  echo -E "@${i}";  echo "ccccaaatcgagttttatcgcttccatgacgcagattagttaacactttcaggat" ; echo "+" ; echo '=======================================================' ; done >> r1.fq
for i in $(seq 26 30) ; do  echo -E "@${i}";  echo "ccccaaaacgagttttatcgcttccatgacgcagcttagttaacactttgaggat" ; echo "+" ; echo '=======================================================' ; done >> r1.fq

bowtie2-build ref.fa ref_index
bowtie2 -x ref_index -U r1.fq -S aligned_reads.sam -D 20 -R 3 -N 1 -L 12 -i S,1,0.50 --score-min L,-0.8,-0.8
samtools view -bS aligned_reads.sam > aligned_reads.bam
samtools sort aligned_reads.bam -o sorted_reads.bam
samtools index sorted_reads.bam
samtools mpileup -f ref.fa -aa -A -d 0 -B -Q 0 sorted_reads.bam > reads.pileup
