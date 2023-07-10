const home = document.querySelectorAll('.nav-home');
const about = document.querySelectorAll('.nav-about');
const projects = document.querySelectorAll('.nav-projects');
const contact = document.querySelectorAll('.nav-contact');
const linkedin = document.querySelectorAll('.nav-linkedin');
const github = document.querySelectorAll('.nav-github');

home.forEach((element) => {
  element.setAttribute('href', 'index.html');
});

about.forEach((element) => {
  element.setAttribute('href', 'about.html');
});

projects.forEach((element) => {
  element.setAttribute('href', 'projects.html');
});

contact.forEach((element) => {
  element.setAttribute('href', 'contact.html');
});

github.forEach((element) => {
  element.setAttribute('href', 'https://github.com/jomacaag');
});

linkedin.forEach((element) => {
  element.setAttribute('href', 'https://www.linkedin.com/in/jonathan-castro-aguilar-aa3372107/');
});
