# GTC
The Program is designed for the presentation of html-markup as a table of contents, where every title is an anchor element.

Input file example:
<hr>
<h1 id="section1">Beetles</h1>
    <h2 id="section2">External morphology</h2>
        <h3 id="section3">Head</h3>
            <h4 id="section4">Mouthparts</h4>
        <h3 id="section5">Thorax</h3>
            <h4 id="section6">Prothorax</h4>
            
Output file example:  
<hr>
<ul>
<li>
<a href="<путь к файлу>/#id="section1">Beetles</a>
</li>
<ul>
<li>
<a href="<путь к файлу>/#id="section2">External morphology</a>
</li>
<ul>
<li>
<a href="<путь к файлу>/#id="section3">Head</a>
</li>
<ul>
<li>
<a href="<путь к файлу>/#<id="section4">Mouthparts</a>
</li>
</ul>
<li>
<a href="<путь к файлу>/#<id=«section5">Thorax</a>
</li>
<ul>
<li>
<a href="<путь к файлу>/#<id="section6">Prothorax </a>
</li>
<li>
