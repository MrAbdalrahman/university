function togglePasswordLogin() {
  const baseUrl = document.body.getAttribute("data-base-url");

  // Get the toggle button and the password input element by their IDs
  const togglePassword = document.getElementById("toggle-password");
  const passwordInput = document.getElementById("password");

  // Check if both elements are present
  if (togglePassword && passwordInput) {
    // Toggle the input type between password and text, showing or hiding the password
    const type =
      passwordInput.getAttribute("type") === "password" ? "text" : "password";
    passwordInput.setAttribute("type", type);

    // Optionally add a class to trigger animation (if needed)
    togglePassword.classList.add("fade");

    // After a delay (to match the animation duration), change the icon and remove the animation class
    setTimeout(() => {
      togglePassword.src =
        type === "password"
          ? baseUrl + "images/eye.png"
          : baseUrl + "images/view.png";
      togglePassword.classList.remove("fade");
    }, 200); // Delay set to 200ms
  } else {
    // Log an error if the elements were not found
    console.error("Element not found!");
  }
}

// Function to check the strength of the password and visually represent it
function checkPasswordStrength() {
  var strengthBar = document.getElementById("password-strength");
  var password = document.getElementById("new-password").value;
  var strength = 0;

  // Increment strength based on various criteria
  if (password.match(/[a-z]+/)) {
    strength += 1;
  }
  if (password.match(/[A-Z]+/)) {
    strength += 1;
  }
  if (password.match(/[0-9]+/)) {
    strength += 1;
  }
  if (password.match(/[\W]+/)) {
    strength += 1;
  }
  if (password.length >= 8) {
    strength += 1;
  }

  // Change the width and color of the strength bar based on the password strength
  switch (strength) {
    case 0:
      strengthBar.style.width = "0%";
      strengthBar.style.backgroundColor = "#dddddd";
      break;
    case 1:
      strengthBar.style.width = "20%";
      strengthBar.style.backgroundColor = "#ff3e36";
      break;
    case 2:
      strengthBar.style.width = "40%";
      strengthBar.style.backgroundColor = "#ff691f";
      break;
    case 3:
      strengthBar.style.width = "60%";
      strengthBar.style.backgroundColor = "#ffda36";
      break;
    case 4:
      strengthBar.style.width = "80%";
      strengthBar.style.backgroundColor = "#0be881";
      break;
    case 5:
      strengthBar.style.width = "100%";
      strengthBar.style.backgroundColor = "#23ad5c";
      break;
  }
}

// Function to toggle the visibility of both the password and confirm password fields
function togglePassword() {
  const baseUrl = document.body.getAttribute("data-base-url");

  var newPassword = document.getElementById("new-password");
  var confirmPassword = document.getElementById("confirm-password");
  var toggle = document.getElementById("toggle-password");

  // Add the fade class to make the icon fade out
  toggle.classList.add("fade");

  // Check the current type and toggle accordingly, including changing the toggle icon
  // Use setTimeout to wait for the fade effect to complete before changing the icon and fading back in
  setTimeout(() => {
    if (newPassword.type === "password") {
      newPassword.type = "text";
      confirmPassword.type = "text";
      toggle.src = baseUrl + "images/view.png"; // Icon for visible password
    } else {
      newPassword.type = "password";
      confirmPassword.type = "password";
      toggle.src = baseUrl + "images/eye.png"; // Icon for hidden password
    }
    // Apply a transform scale to create a motion effect
    toggle.style.transform = "scale(1.2)";
    // Remove the fade class to fade the icon back in and reset transform after a short delay
    setTimeout(() => {
      toggle.classList.remove("fade");
      toggle.style.transform = "scale(1)"; // Reset the scale back to normal
    }, 50); // Adjust timing as needed
  }, 150); // This should match the duration of the fade effect
}

// Function to check if the new password and confirm password inputs match
function checkPasswordMatch() {
  var newPassword = document.getElementById("new-password").value;
  var confirmPassword = document.getElementById("confirm-password").value;
  var passwordInfo = document.getElementById("password-match-info");

  // Display a message if passwords do not match
  if (newPassword && confirmPassword && newPassword !== confirmPassword) {
    passwordInfo.textContent = "Passwords do not match.";
    passwordInfo.style.color = "red";
  } else {
    // Clear the message when passwords match or fields are empty
    passwordInfo.textContent = newPassword && confirmPassword ? "" : "";
    passwordInfo.style.color = "green";
  }
}
