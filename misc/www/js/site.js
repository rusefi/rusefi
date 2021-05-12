(function($) { // Begin jQuery
  $(function() { // DOM ready
    // If a link has a dropdown, add sub menu toggle.
    $('topnav ul li a:not(:only-child)').click(function(e) {
      $(this).siblings('.nav-dropdown').toggle();
      // Close one dropdown when selecting another
      $('.nav-dropdown').not($(this).siblings()).hide();
      e.stopPropagation();
    });
    // Clicking away from dropdown will remove the dropdown class
    $('html').click(function() {
      $('.nav-dropdown').hide();
    });
    // Toggle open and close nav styles on click
    $('#nav-toggle').click(function() {
      $('topnav ul').slideToggle();
    });
    $('topnav a').click(function() {
      $('topnav ul').slideToggle();
      $('#nav-toggle').removeClass('active');
    });
    // Hamburger to X toggle
    $('#nav-toggle').on('click', function() {
      this.classList.toggle('active');
    });
  }); // end DOM ready
})(jQuery); // end jQuery

$(document).ready(function() {

    // Variables
    var $codeSnippets = $('.code-example-body'),
        $nav = $('.navbar'),
        $body = $('body'),
        $window = $(window),
        $popoverLink = $('[data-popover]'),
        navOffsetTop = $nav.offset().top,
        $document = $(document),
        entityMap = {
          "&": "&amp;",
          "<": "&lt;",
          ">": "&gt;",
          '"': '&quot;',
          "'": '&#39;',
          "/": '&#x2F;'
        }
  
    function init() {
      $window.on('scroll', onScroll)
      $window.on('resize', resize)
      $popoverLink.on('click', openPopover)
      $document.on('click', closePopover)
      $('a[href^="#"]').on('click', smoothScroll)
      // buildSnippets();
    }
  
    function smoothScroll(e) {
      e.preventDefault();
      $(document).off("scroll");
      var target = this.hash,
          menu = target;
      $target = $(target);
      $('html, body').stop().animate({
          'scrollTop': $target.offset().top-40
      }, 0, 'swing', function () {
          window.location.hash = target;
          $(document).on("scroll", onScroll);
      });
    }
  
    function openPopover(e) {
      e.preventDefault()
      closePopover();
      var popover = $($(this).data('popover'));
      popover.toggleClass('open')
      e.stopImmediatePropagation();
    }
  
    function closePopover(e) {
      if($('.popover.open').length > 0) {
        $('.popover').removeClass('open')
      }
    }
  
    $("#button").click(function() {
      $('html, body').animate({
          scrollTop: $("#elementtoScrollToID").offset().top
      }, 2000);
  });
  
    function resize() {
      $body.removeClass('has-docked-nav')
      navOffsetTop = $nav.offset().top
      onScroll()
    }
  
    function onScroll() {
      if(navOffsetTop < $window.scrollTop() && !$body.hasClass('has-docked-nav')) {
        $body.addClass('has-docked-nav')
      }
      if(navOffsetTop > $window.scrollTop() && $body.hasClass('has-docked-nav')) {
        $body.removeClass('has-docked-nav')
      }
    }
  
    function escapeHtml(string) {
      return String(string).replace(/[&<>"'\/]/g, function (s) {
        return entityMap[s];
      });
    }
  
    // function buildSnippets() {
    //   $codeSnippets.each(function() {
    //     var newContent = escapeHtml($(this).html())
    //     $(this).html(newContent)
    //   })
    // }
  
  
    init();
  
  });