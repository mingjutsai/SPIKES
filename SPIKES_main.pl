#!/usr/bin/perl
use strict;
use warnings;
my $input = $ARGV[0];
my $output_dir = $ARGV[1];
if(@ARGV < 2){
    print STDERR "SPIKES_main.pl protein.fa output_dir\n";die;
}
$output_dir =~ s/\/$//;
my $aaindex = "src/aaindex/aa";
my $svm_predict = "src/libsvm_320/svm-predict";
my $svm_scale = "src/libsvm_320/svm-scale";
my $svm_scale_range = "src/libsvm_320/svm_scale_range";
my $fasta2seq = "src/fasta2seq.pl";
if(!-e $aaindex){
    print STDERR "Please build the aaindex program first\n";die;
}
if(!-e $svm_predict){
    print STDERR "Please build the svm-predict program first\n";die;
}
if(!-e $svm_scale){
    print STDERR "Please build the svm-scale program first\n";die;
}
my $tmp_dir = $output_dir."/tmp";
my $cmd = "mkdir ".$tmp_dir;
`$cmd`;
$cmd = "perl src/fasta2seq.pl ".$input." ".$tmp_dir;
`$cmd`;
my $seq =  $tmp_dir."/".$input."_seq";
my $seq_aa = $tmp_dir."/".$input."_aa";
my $seq_aa_scale = $tmp_dir."/".$input."_scale";
my $seq_aa_scale_feature_select = $seq_aa_scale."_features.tab";
my $svm_predict_output = $seq_aa_scale_feature_select.".predict";
$cmd = $aaindex." -a 0 ".$seq." > ".$seq_aa;
`$cmd`;
$cmd = $svm_scale." -r ".$svm_scale_range." ".$seq_aa." > ".$seq_aa_scale;
`$cmd`;
$cmd = "perl src/FeatureSelect_data.pl -data ".$seq_aa_scale." -tab src/libsvm_320/features.tab";
`$cmd`;
$cmd = $svm_predict." -b 1 ".$seq_aa_scale_feature_select." SPIKES.model ".$svm_predict_output;
`$cmd`;
$cmd = "perl src/merge_results.pl ".$input." ".$svm_predict_output." ".$output_dir;
print STDERR $cmd."\n";
`$cmd`;



