<h1>NaviDir</h1>
A simple terminal based file explorer</li>

<h1>How to run ?</h1>
<ul>
  <li><code>git clone https://github.com/pushkar-gr/NaviDir.git</code></li>
  <li><code>git clone https://github.com/ArthurSonzogni/FTXUI.git lib/FTXUI</code></li>
  <li>install cmake and gtest</li>
  <li><code>mkdir build</code></li>
  <li><code>cmake -S . -B build</code></li>
  <li><code>cmake --build build</code></li>
  <li><code>build/bin/NaviDir</code></li>
</ul>

<h1>Pre-requisite: </h1>
<ul>
  <li>Any nerd font installed</li>
</ul>

<h1>How to use ?</h1>
<ul>
  <li><code>h</code>: switch to parent directory</li>
  <li><code>l</code>: switch to selected file</li>
  <li><code>k</code>: select previous file</li>
  <li><code>j</code>: select next file</li>
  <li><code>Ctrl + h</code>: scroll left</li>
  <li><code>Ctrl + l</code>: scroll right</li>
  <li><code>Ctrl + k</code>: scroll up</li>
  <li><code>Ctrl + j</code>: scroll down</li>
  <li><code>a</code>: create new file</li>
  <li><code>e</code>: rename file name</li>
  <li><code>r</code>: rename file</li>
  <li><code>u</code>: move file</li>
  <li><code>d</code>: delete file</li>
  <li><code>y</code>: copy file</li>
  <li><code>Shift + y</code>: cut file</li>
  <li><code>p</code>: paste file</li>
  <li><code>f</code>: filter files</li>
  <li><code>v</code>: toggle hidden file</li>
  <li><code>b</code>: toggle file permissions</li>
  <li><code>n</code>: toggle file size</li>
  <li><code>m</code>: toggle modification date</li>
  <li><code>q</code>: quit</li>
</ul>

<h1>How to contribute ?</h1>
<ul>
  <li>Clone the repo</li>
  <li>Make changes</li>
  <li>Write approtiate tests</li>
  <li>Build tests: <code>cmake --build build --target NaviDirTests</code></li>
  <li>Pass all tets (<code>build/bin/NaviDirTests</code>)</li>
  <li>Make a PR</li>
</ul>
