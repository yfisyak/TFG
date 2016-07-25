#! /usr/bin/env perl
# convert make class to declaration for TTreeIter
if ($#ARGV !=0) {
  print "usage $0 class.h\n";
 exit;
}
open (In,"<$ARGV[0]") or die "Can't open input $ARGV[0]";
my $Output = $ARGV[0] . ".hh";
my $line;
my $declaration = 0;
my $classname = "";
my @LoH = ();
while ($line = <In>) {
  next if ! $line;
#  if ($line =~ /Declaration of leave types/) {$declaration = 1; next;}
  if ($line =~ /^class/) {
    my ($d1,$d2,$d3) = split ' ',$line;
    $classname = $d2; #print "$line class name = $classname\n";
  }
  if ($line =~ /Declaration of leaf types/ || $line =~ /Declaration of leave types/) {$declaration = 1; next;}
  if ($line =~ /List of branches/) {$declaration = 2; next;}
  next if ! $declaration;
  next if $declaration == 2 and $line !~ /SetBranchAddress/;
  next if $line =~ /^\s*$/;
  next if $line =~ /^\s*\/\//;
#  next if $line !~ /fUniqueID/;
#  print $line;
  if ($declaration == 1) {
    my ($type,$variable) = split ' ',$line;# print "type = $type , variable = $variable\n";
    my $rec = {};
    if ($variable =~ /\[/) {
      $type .= "*&";
      $variable =~ s/\[.*//;
    } else {
      $type .= "&";
      $variable =~ s/\;.*//;
    }
    $rec->{type} = $type;
    $rec->{Variable} = $variable;
    $rec->{Branch} = "";
#    print "type = |$rec->{type}| Variable = |$rec->{Variable}| Branch = |$rec->{Branch}|\n";
    push @LoH, $rec;
    next;
  }
  my ($dum,$name,$rest) = split '"', $line;# print "name = |$name|\n";
  my ($dum,$var)        = split ',', $rest; 
  $var =~ s/ //g;
  $var =~ s/\&//;
  $var =~ s/\;//;# print "var  = |$var|\n";
  foreach my $r (@LoH) {
#    print "var = |$var|    --- type = |$r->{type}| Variable = |$r->{Variable}| Branch = |$r->{Branch}|\n";
#    print "|$var|  Variable = |$r->{Variable}|\n";
    if ($r->{Variable} eq $var) {
      $r->{Branch} = $name;
#      print "type = |$r->{type}| Variable = |$r->{Variable}| Branch = |$r->{Branch}|\n";
      last;
    }
  }
}
close (In);
open (Out, ">$Output") or die "Can't open output $Output";
print      "class $classname {\n\tpublic:\n\tChar_t             mBeg[1];        //!\n";
print Out  "class $classname {\n\tpublic:\n\tChar_t             mBeg[1];        //!\n";
foreach my $rec (@LoH) {
  next if ! $rec->{Branch};
  printf     "\tconst %-12s %s;\n", $rec->{type}, $rec->{Variable};
  printf Out "\tconst %-12s %s;\n", $rec->{type}, $rec->{Variable};
}
print     "\tChar_t             mEnd[1];        //!\n\t$classname(TTreeIter &iter) :\n";
print Out "\tChar_t             mEnd[1];        //!\n\t$classname(TTreeIter &iter) :\n";

foreach my $rec (@LoH) {
  next if ! $rec->{Branch};
  printf     "\t%s(iter(\"%s\")),\n", $rec->{Variable}, $rec->{Branch};
  printf Out "\t%s(iter(\"%s\")),\n", $rec->{Variable}, $rec->{Branch};
}
print     "\tmEnd(0){}\n;\n";
print Out "\tmEnd(0){}\n};\n";
close(Out);
