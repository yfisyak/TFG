#! /usr/bin/env perl
#my @list = `get_file_list.pl -delim '/' -keys 'path,filename,events' -cond 'filetype=online_daq,filename~st_laser,tpx=1,sanity=1,runnumber>15000000,events>10' -limit 0`;
my @list = `get_file_list.pl -delim '/' -keys 'path,filename' -cond 'filetype=online_daq,filename~st_laser,tpx=1,sanity=1,runnumber>15000000,events>10' -limit 0`;

foreach my $file (@list) {
  chomp($file);
  my $basename = $file;
  $basename =~ s#/home/starsink/raw##;
#  print "$file = > $basename\n";
  my $found = 0;
  my $dir;
  my @dirs = qw(/star/data03 /gpfs01/scratch/users/fisyak);
  foreach $dir (@dirs) {
    my $fullpath = $dir . $basename;
 #   print "$fullpath ===========";
    if (-r $fullpath) {
      $found = 1; 
      #print "found\n"; 
      last;
    }
#    print "not found\n";
  }
  if (! $found) {
    print "$file $dirs[1]$basename\n";
  }
}
