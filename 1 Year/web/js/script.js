(() => {
  const modals = Array.from(document.querySelectorAll('[data-auth-modal]'));

  if (modals.length === 0) {
    return;
  }

  const modalByMode = new Map();
  modals.forEach((modal) => {
    const mode = modal.getAttribute('data-auth-modal');
    if (mode) {
      modalByMode.set(mode, modal);
    }
  });

  const openButtons = Array.from(document.querySelectorAll('.js-open-auth'));
  const closeButtons = Array.from(document.querySelectorAll('[data-auth-close]'));
  const switchButtons = Array.from(document.querySelectorAll('[data-auth-switch]'));
  const forms = Array.from(document.querySelectorAll('.auth-form'));

  const modeConfig = {
    login: {
      success: 'Login successful. Your Finex dashboard is ready.'
    },
    signup: {
      success: 'Account created successfully. Welcome to Finex Bank.'
    }
  };

  const closeAllModals = () => {
    modals.forEach((modal) => {
      modal.hidden = true;
    });
    document.body.classList.remove('auth-open');
  };

  const focusFirstField = (modal) => {
    const firstField = modal ? modal.querySelector('.auth-input') : null;

    if (firstField) {
      firstField.focus();
    }
  };

  const openModal = (mode) => {
    const nextMode = modeConfig[mode] ? mode : 'login';
    const nextModal = modalByMode.get(nextMode);

    if (!nextModal) {
      return;
    }

    closeAllModals();
    nextModal.hidden = false;
    document.body.classList.add('auth-open');
    focusFirstField(nextModal);
  };

  openButtons.forEach((button) => {
    button.addEventListener('click', () => {
      const mode = button.getAttribute('data-auth-mode');
      openModal(mode);
    });
  });

  closeButtons.forEach((button) => {
    button.addEventListener('click', closeAllModals);
  });

  switchButtons.forEach((button) => {
    button.addEventListener('click', () => {
      const targetMode = button.getAttribute('data-auth-switch');
      openModal(targetMode);
    });
  });

  forms.forEach((form) => {
    form.addEventListener('submit', (event) => {
      event.preventDefault();

      if (!form.reportValidity()) {
        return;
      }

      const mode = form.getAttribute('data-auth-form');
      const modal = form.closest('[data-auth-modal]');
      const feedbackElement = modal ? modal.querySelector('.auth-feedback') : null;
      const modeName = modeConfig[mode] ? mode : 'login';

      if (feedbackElement) {
        feedbackElement.textContent = modeConfig[modeName].success;
      }

      form.reset();

      window.setTimeout(() => {
        closeAllModals();
      }, 900);
    });
  });

  document.addEventListener('keydown', (event) => {
    const hasOpenModal = modals.some((modal) => !modal.hidden);

    if (!hasOpenModal) {
      return;
    }

    if (event.key === 'Escape') {
      closeAllModals();
    }
  });
})();
