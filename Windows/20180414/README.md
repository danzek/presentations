# BITS and pieces: Abusing BITS for persistence and privilege escalation

## Abstract

As incident responders / reverse engineers, we often learn new things about how Windows works from malware authors. We’ll share how threat actors are leveraging the Windows Background Intelligent Transfer Service (BITS) for persistence and privilege escalation. We’ll present proof-of-concept code demonstrating how this could be abused further, and we’ll show what you’d expect to see both from static and dynamic reverse engineering of this code as well as system artifacts. We’ll be sharing what we’ve learned in our experiences and research in a way that benefits both blue and red team members (insert your favorite "purple team" one-liner here).

## Links

 - [Recorded talk (BSides Iowa)](https://youtu.be/8yFj35x0WFQ)
 - [View original slides on Prezi](https://prezi.com/view/WcB25JR7W7TeEh6Yvue7/) [[PDF version]](Bits_and_pieces.pdf)
 - [Proof-of-concept code (C++)](BitsPoC.cpp)
 - [Slides for EvolveSec Meetup](https://prezi.com/view/FhUhEgGP4CgOduNC5SAd/)

## Talk given on the following occasions

Ilya Kobzar and I created this talk for BSides Iowa 2018. We or I gave this talk on the following occasions:

 - Thursday, April 12, 2018 at the Illinois Institute of Technology (IIT) Rice campus in Wheaton, IL (for the Midwest Chapter meeting of the HTCIA and ForenSecure conference)
 - Saturday, April 14, 2018 at BSides Iowa at Grand View University in Des Moines, IA
 - Tuesday, August 28, 2018 at the EvolveSec Meetup in Chicago, IL
 - Friday/Saturday, October 12-13, 2018 at HackUMass at University of Massachusetts in Amherst, MA
